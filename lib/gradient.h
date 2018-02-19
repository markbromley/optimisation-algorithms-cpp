#ifndef GRADIENT_H
#define GRADIENT_H

// std::setprecision
#include <iomanip>
#include <limits>

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <utility>
#include <memory>

// input parameters
#include "parameter.hh"

// Graphs
#include "graphplotter.hh"

// Optimisation datapoint
#include "datapoint.h"
#include "linearerrorfunction.h"

// sum squares error function
// #include "../wormerrorfunction.hh"

// #include "../vector.hh"

class GradientDescent
{
    private:

        // Initial warnings shown
        bool warnings_shown = false;

        // Internal storage of the theta variables
        std::vector<double> m_theta;

        // Internal storage of the error values
        std::vector<double> m_error_values;

        // Internal storage of the training examples
        std::vector<DataPoint> m_data_points;

        // Store the config params, if available
        std::shared_ptr<ConfigParameters> m_config_params = nullptr;
        
        // TODO: These shouldn't be necessary...
        unsigned int number_of_training_points;
        unsigned int number_of_features;

        // Error function
        std::shared_ptr<ErrorFunction> m_err_func;

        bool _convergence_function(std::vector<double> new_theta,
                                  double current_error,
                                  bool can_converge,
                                  double epsilon)
        {
            // Currently has fixed minimum value for if stuck in saddle point
            // whilst high error, i.e. converge is false when high error even
            // if converging
            double acceptable_error;
            double theta_convergence;
            if(this->m_config_params != nullptr)
            {
                acceptable_error = m_config_params->get<double>("optimisation.grad.acceptable_error");
                theta_convergence = m_config_params->get<double>("optimisation.grad.theta_convergence");
            }
            else
            {
                if(!this->warnings_shown)
                {
                    std::cout << "Warning: No parameters passed, using base value for" <<
                    " acceptable error and theta convergence." << std::endl;
                }
                acceptable_error = 0.00001;
                theta_convergence = 0.01;
                warnings_shown = true;
            }

            // Stop early
            if(current_error < acceptable_error)
            {
                std::cout << "Current error below manual acceptable threshold level." << std::endl;
                return true;
            }

            // Stop if theta params stopped changing
            can_converge = current_error < theta_convergence;
            for (unsigned int i = 0; i < m_theta.size(); i++)
            {
                can_converge = can_converge && fabs(m_theta[i]-new_theta[i]) < epsilon;
            }
            return can_converge;
        }

        void _update_error_logs(unsigned int number_of_iterations, double current_error)
        {
            // Only log the error every N iterations, otherwise vector's huge (i.e. GB!)
            unsigned int log_err_modulo;
            unsigned int print_err_modulo;
            if(this->m_config_params != nullptr)
            {
                log_err_modulo = m_config_params->get<unsigned int>("optimisation.log_err_modulo");
                print_err_modulo = m_config_params->get<unsigned int>("optimisation.print_err_modulo");
            }
            else
            {
                if(!this->warnings_shown)
                {
                    std::cout << "Warning: No parameters passed, using base value for error module." << std::endl;
                }
                log_err_modulo = 1;
                print_err_modulo = 1;
            }

            if(number_of_iterations % log_err_modulo == 0)
            {
                this->logError(current_error);
            }
            if (number_of_iterations % print_err_modulo == 0)
            {
                int precision = std::numeric_limits<double>::max_digits10;
                std::cout << std::setprecision(precision) << "Error is: " << current_error << std::endl;
            }      
       }

    public:
        explicit GradientDescent(std::vector<DataPoint>& data_point_samples,
                                 std::vector<double>& initial_theta,
                                 std::pair<double, double> minMaxPair,
                                 std::shared_ptr<ConfigParameters> config_params = nullptr)
        {
            // Store the config params
            this->m_config_params = config_params;

            // Set sizes
            number_of_features = 0;
            m_data_points = data_point_samples;
            number_of_training_points = data_point_samples.size();
            if (number_of_training_points > 0)
            {
                number_of_features = data_point_samples[0].getFeatures().size();
            }

            // Initialise theta values
            m_theta = std::vector<double>(number_of_features, 1.0);
            for(unsigned int i = 0; i < initial_theta.size(); i++)
            {
                // Adjust value here if necessary
                m_theta[i] = initial_theta[i];
            }

            // Open the error function
            m_err_func = std::shared_ptr<ErrorFunction>(new LinearErrorFunction());
            m_err_func->add_params(config_params);
            m_err_func->add_min_max_pair(minMaxPair);
        }

        std::vector<double> run()
        {
            // Initial learning rate (0.0000001)
            double alpha;

            // Convergence delta epsilon (0.00001)
            double eps;

            // Percentage by which to adjust learning rate at each iteration
            float adaptive_learning_rate;

            // Momentum rate
            float momentum_gamma;

            // Set using file if available, or defaults otherwise
            if(this->m_config_params != nullptr)
            {
                alpha = m_config_params->get<double>("optimisation.grad.alpha");
                eps = m_config_params->get<double>("optimisation.grad.eps");
                adaptive_learning_rate = m_config_params->get<double>("optimisation.grad.adaptive_learning_rate");
                momentum_gamma = m_config_params->get<double>("optimisation.grad.momentum_gamma");
            }
            else
            {
                std::cout << "Warning: No parameters passed, using base gradient descent settings." << std::endl;
                alpha = 0.01;
                eps = 0.00001;
                adaptive_learning_rate = 0.05;
                momentum_gamma = 0.9;
            }
            
            // Used for monitoring the error rate (adaptive learning)
            double previous_error = std::numeric_limits<double>::max();

            // Internal momentum flag/ 
            std::vector<double> previous_j_theta_deriv = std::vector<double>(number_of_features, 1.0);
            bool is_previous_j_theta_deriv_set = false;

            // Set to true, once the hypothesis parameters change by less than epsilon
            bool converge = false;

            // Count of the number of iterations
            unsigned int number_of_iterations = 0;

            // Used for storing the current iteration's error, so not recalculated
            double current_error = std::numeric_limits<double>::max();

            while (!converge)
            {
                std::vector<double> newTheta = m_theta;

                // Calculate the current error for this iteration
                current_error = this->m_err_func->error_function(m_theta, m_data_points);


                // Log the errors (print/ graph etc.)
                this->_update_error_logs(number_of_iterations, current_error);

                // Compare error rate and use adaptive learning rate
                if(previous_error > current_error)
                {
                    // If the error is reducing accelerate learning rate
                    alpha *= (1 + adaptive_learning_rate);
                }
                else
                {
                    // If the error is increasing or stationary, reduce learning rate
                    alpha *= (1 - adaptive_learning_rate);
                }

                // Update tab of the error
                previous_error = current_error;

                for (unsigned int i = 0; i < number_of_training_points; i++)
                {
                    // Single training exaple
                    DataPoint data_point = m_data_points[i];

                    // Calculate the partial derivatives of the error function w.r.t.
                    // to each parameter
                    auto all_j_theta_derivs = this->m_err_func->error_function_derivative(data_point, newTheta);

                    // Apply the learning rate to each feature and add in momentum
                    for (unsigned int j = 0; j < number_of_features; j++)
                    {
                        // Learning rate, alpha
                        double j_theta_deriv = all_j_theta_derivs[j];
                        j_theta_deriv *= alpha;

                        // Momentum previous theta initialisation for first run
                        if(!is_previous_j_theta_deriv_set)
                        {
                            previous_j_theta_deriv[j] = j_theta_deriv;
                        }

                        // Add in momentum
                        j_theta_deriv = (momentum_gamma * previous_j_theta_deriv[j]) + j_theta_deriv;

                        // Perform the update
                        newTheta[j] -= j_theta_deriv;
                        previous_j_theta_deriv[j] = j_theta_deriv;
                    }

                    // After first iteration, can use previous j_theta values for i-1
                    // value in momentum
                    is_previous_j_theta_deriv_set = true;

                }

                // Have the parameters converged?
                converge = this->_convergence_function(newTheta, current_error, converge, eps);

                m_theta = newTheta;
                number_of_iterations++;
            }

            return m_theta;
        }

        void printData()
        {
            for(unsigned int i = 0; i < number_of_training_points; i++)
            {
                DataPoint data_point = m_data_points[i];
                // TODO: Add more detail
                std::cout << "Example: " << i << " Target: " << data_point.getTarget(0) << std::endl;
            }
        }

       void plot_error_graphs(bool display = true)
       {
            std::cout << "Error Vector size: " << this->m_error_values.size() << std::endl;
            std::shared_ptr<GraphPlotter> graph(new GraphPlotter);
            // Where to save the graphs
            std::string path = this->m_config_params->getString("optimisation.err_graph_location");
            // Plot log error
            bool use_log_error = true;
            std::string log_file_path("./" + path + "log_error_plot.png");
            graph->plot_error_graph(this->m_error_values, log_file_path.c_str(), display, use_log_error, 0);

            // Plot the none log error
            use_log_error = false;
            std::string err_file_path( "./" + path + "full_plot.png");
            graph->plot_error_graph(this->m_error_values, err_file_path.c_str(), display, use_log_error, 0);
       }

       // Record the error
       void logError(double error_value)
       {
            this->m_error_values.push_back(error_value);
       }
};

#endif
