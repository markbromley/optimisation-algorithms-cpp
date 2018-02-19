#ifndef LIN_ERR_FUNC_HH
#define LIN_ERR_FUNC_HH

#include <vector>

// Optimisation datapoint
#include "datapoint.h"

#include "errorfunction.h"

// input parameters
#include "parameter.hh"

class LinearErrorFunction : public ErrorFunction
{
private:
    // The config params
    std::shared_ptr<ConfigParameters> m_params;

    // The normalised min/ max
    std::pair<double, double> m_data_minMaxPair;

    // Hypothesis e.g. Model
    std::vector<double> hypothesis(std::vector<double>& theta, 
                                   std::vector<double>& features)
    {
        double sum = 0.0;
        for (unsigned int i = 0; i < features.size(); i++)
        {
            sum += theta[i]*features[i];
        }
        return std::vector<double>(1, sum);
    }

public:

    // Error Function e.g. SSE
    double error_function(std::vector<double>& theta, 
                          std::vector<DataPoint>& data_points)
    {
        double sum = 0.0;
        for (unsigned int i = 0; i < data_points.size(); i++)
        {
            double diff_1 = this->hypothesis(theta, data_points[i].getFeatures())[0] - 
                data_points[i].getTarget(0);
            sum += diff_1 * diff_1;
        }
        return sum / 2.0;
    }
    
    // Error Function derivative (exact / numerical)
    std::vector<double> error_function_derivative (DataPoint data_point, 
                                                   std::vector<double> theta)
    {
        unsigned int number_of_features = data_point.getFeatures().size();
        // Prepare vector of J_Theta derivatives for each feature
        auto all_j_theta_derivs = std::vector<double>(number_of_features, 1.0);
        std::vector<double> h_theta = this->hypothesis(theta, data_point.getFeatures());
        double difference = h_theta[0] - data_point.getTarget(0);
        for (unsigned int i = 0; i < number_of_features; i++)
        {
            double j_theta_deriv = difference * data_point.getFeature(i);
            all_j_theta_derivs[i] = j_theta_deriv;
        }
        return all_j_theta_derivs;
    }

    // Add a pointer to the params
    void add_params(std::shared_ptr<ConfigParameters> params)
    {
        this->m_params = params;
        if(params != nullptr)
        {
            const double a = params->get<double>( "mesh.a" );
            std::cout << "Mesh A is: " << a << std::endl;
        }else{
            std::cout << "Null pointer" << std::endl;
        }
    }

    // Set the internal record of the normalised min/ max values
    void add_min_max_pair(std::pair<double, double> minMaxPair)
    {
        this->m_data_minMaxPair = minMaxPair;
    }
};

#endif