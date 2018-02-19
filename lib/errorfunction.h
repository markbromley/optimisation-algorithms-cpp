#ifndef ERR_FUNC_HH
#define ERR_FUNC_HH

#include <vector>

// Optimisation datapoint
#include "datapoint.h"

// input parameters
#include "parameter.hh"


class ErrorFunction
{
private:

    // Hypothesis e.g. Model
    virtual std::vector<double> hypothesis(std::vector<double>& theta, 
                                           std::vector<double>& features) = 0;
public:

    // Error Function e.g. SSE
    virtual double error_function(std::vector<double>& theta, 
                                  std::vector<DataPoint>& data_points) = 0;
    
    // Error Function derivative (exact / numerical)
    virtual std::vector<double> error_function_derivative (DataPoint data_point, 
                                                           std::vector<double> theta) = 0;

    // These should be set before calling error function or derivative, (only if they are used)
    virtual void add_params(std::shared_ptr<ConfigParameters> params) = 0;

    // Add the min/ max pair
    virtual void add_min_max_pair(std::pair<double, double> minMaxPair) = 0;

};

#endif