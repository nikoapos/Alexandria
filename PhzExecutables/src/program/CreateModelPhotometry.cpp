/** 
 * @file CreateModelPhotometry.cpp
 * @date May 22, 2014
 * @author Nikolaos Apostolakos
 */

#define SVN_ID "SVN $Id$"
#define SVN_URL "SVN $HeadURL$"

#include <string>
#include <boost/program_options.hpp>
#include <fstream>
namespace po = boost::program_options;
#include "ElementsKernel/ElementsProgram.h"
#include "ElementsKernel/Version.h"
#include "ChMath/function/Function.h"
#include "ChMath/function/function_tools.h"
#include "PhzConfiguration/ModelingConfiguration.h"
#include "PhzDataModel/PhzModel.h"
#include "PhzDataModel/PhotometryMatrix.h"
#include "PhzModeling/ModelMatrix.h"

using namespace std;

class CreateModelPhotometry : public ElementsProgram {

public:
  
  CreateModelPhotometry() = default;
  
  virtual ~CreateModelPhotometry() = default;
  
  po::options_description defineSpecificProgramOptions() {
    po::options_description config_file_options("Model Photometry options");
    config_file_options.add_options()
    ("binary-photometry-matrix", po::value<string>(),
        "The file to export in binary format the matrix containing the calculated photometries")
    ("sed-root-path", po::value<string>(),
        "The directory containing the SED datasets, organized in folders")
    ("sed-group", po::value<vector<string>>(),
        "Use all the SEDs in the given group and subgroups")
    ("sed-list", po::value<vector<string>>(),
        "Use all the given SEDs")
    ("reddening-curve-root-path", po::value<string>(),
        "The directory containing the Reddening Curve datasets, organized in folders")
    ("reddening-curve-group", po::value<vector<string>>(),
        "Use all the Reddening Curves in the given group and subgroups")
    ("reddening-curve-list", po::value<vector<string>>(),
        "Use all the given Reddening Curves")
    ("ebv-start", po::value<double>(), "The E(B-V) range lower limit")
    ("ebv-stop", po::value<double>(), "The E(B-V) range upper limit")
    ("ebv-step", po::value<double>(), "The E(B-V) step")
    ("z-start", po::value<double>(), "The redshift range lower limit")
    ("z-stop", po::value<double>(), "The redshift range upper limit")
    ("z-step", po::value<double>(), "The redshift step")
    ("filter-root-path", po::value<string>(),
        "The directory containing the Filter datasets, organized in folders")
    ("filter-group", po::value<vector<string>>(),
        "Use all the Filters in the given group and subgroups")
    ("filter-list", po::value<vector<string>>(),
        "Use all the given Filters");
    return config_file_options;
  }
  
  void mainMethod() {
    ElementsLogging logger = ElementsLogging::getLogger("CreateModelPhotometry");
    
    const po::variables_map options = this->getVariablesMap();
    
    PhzConfiguration::ModelingConfiguration config {std::move(options)};
    
    auto axes_tuple = PhzDataModel::createAxesTuple(config.zList(), config.ebvList(),
                                  config.reddeningCurveList(), config.sedList());

    std::unique_ptr<PhzModeling::ModelDataManager> model_function_manager {
            new PhzModeling::ModelDataManager {axes_tuple,
                config.sedDatasetProvider(), config.reddeningCurveDatasetProvider()}};
    
    PhzModeling::ModelMatrix model_matrix {std::move(model_function_manager), axes_tuple};
                
    auto filter_provider = config.filterDatasetProvider();
    auto filter_list = config.filterList();
    auto filter_name_list_ptr = std::make_shared<std::vector<std::string>>();
    std::vector<std::unique_ptr<ChMath::Function>> filter_functions;
    std::vector<double> filter_compensations;
    std::vector<std::pair<double,double>> filter_limits;
    for (auto& filter : filter_list) {
      auto filter_dataset = filter_provider->getDataset(filter);
      filter_functions.push_back(ChMath::interpolate(*filter_dataset, ChMath::InterpolationType::LINEAR));
      filter_name_list_ptr->push_back(filter.qualifiedName());
      std::vector<double> x;
      std::vector<double> y;
      for (auto& pair : *filter_dataset) {
        x.push_back(pair.first);
        y.push_back(pair.second * 2.99792458e+18 / (pair.first*pair.first));
      }
      filter_limits.push_back(std::make_pair(x.front(), x.back()));
      auto filter_comp_func = ChMath::interpolate(x, y, ChMath::InterpolationType::LINEAR);
      filter_compensations.push_back(ChMath::integrate(*filter_comp_func, 0., 120000.));
    }
                
    logger.info() << "Number of models to create photometry for: " << model_matrix.size();
    int counter {0};
    std::unique_ptr<std::vector<ChCatalog::Photometry>> photometry_vector {new std::vector<ChCatalog::Photometry>{}};
    for (auto& model : model_matrix) {
      ++counter;
      if (counter%1000 == 0) {
        logger.info() << "Number of models proccessed: " << counter;
      }
      std::vector<ChCatalog::FluxErrorPair> photometry_values;
      for (size_t filter_index=0; filter_index<filter_list.size(); ++filter_index) {
        ChMath::Function& filter_function = *(filter_functions[filter_index]);
        vector<double> x {};
        vector<double> y {};
        auto& limits = filter_limits[filter_index];
        for (auto& pair : model) {
          if (pair.first >= limits.first && pair.first <= limits.second) {
            x.push_back(pair.first);
            y.push_back(pair.second*filter_function(pair.first));
          }
        }
        auto filtered_model = ChMath::interpolate(x, y, ChMath::InterpolationType::LINEAR);
        double flux = ChMath::integrate(*filtered_model, limits.first, limits.second);
        flux = flux / filter_compensations[filter_index];
        photometry_values.push_back({flux, 0.});
      }
      photometry_vector->push_back(ChCatalog::Photometry{filter_name_list_ptr, std::move(photometry_values)});
    }
    
    PhzDataModel::PhotometryMatrix photometry_matrix {std::move(photometry_vector), axes_tuple};
    {
      std::ofstream out {options["binary-photometry-matrix"].as<std::string>()};
      ChMatrix::binaryExport(out, photometry_matrix);
    }
  }
  
  string getVersion() {
    return getVersionFromSvnKeywords(SVN_URL, SVN_ID);
  }

}; // end of class CreateModelPhotometry

MAIN_FOR(CreateModelPhotometry)