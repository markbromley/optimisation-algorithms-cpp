#ifndef GRAPH_PLOTTER_HH
#define GRAPH_PLOTTER_HH

#include <vector>
#include <string>
#include <iostream>

// #include "third-party/matplotlibcpp.h"

class GraphPlotter
{
private:
public:
    explicit GraphPlotter(){}

    void plot_error_graph(std::vector<double> error_vector,
                          const char* save_title,
                          bool display_graph = true,
                          bool log_error = false,
                          double graph_end_trim_percentage = 0.1)
    {
        // if(log_error)
        // {
        //     std::vector<double>log_errors(error_vector.size());
        //     // Plot the log error
        //     for(unsigned int i = 0; i < error_vector.size(); i++)
        //     {
        //         log_errors[i] = std::log10(error_vector[i]);
        //     }
        //     error_vector = log_errors;
        // }

        // int size = error_vector.size();
        // int ten_percent_size = size * graph_end_trim_percentage;
        // std::vector<double>::const_iterator first = error_vector.begin() + ten_percent_size;
        // std::vector<double>::const_iterator last = error_vector.end() - ten_percent_size;
        // std::vector<double> trimmed_vector(first, last);
        // // Construct Name for plot
        // int number_displayed = (size - (2*ten_percent_size));
        
        // // Clear the plot
        // matplotlibcpp::clf();

        // std::string plot_name = "Error (" + std::to_string(size) + 
        //     " iterations, " + std::to_string(number_displayed) + " displayed)";
        // // Named plot
        // matplotlibcpp::named_plot(plot_name, trimmed_vector);
        // // Add graph title
        // matplotlibcpp::title("Gradient Descent (Error per Iteration)");
        // matplotlibcpp::xlabel("Iteration Number");
        // if(log_error)
        // {
        //     matplotlibcpp::ylabel("log(L2 Error)");
        // }
        // else
        // {
        //     matplotlibcpp::ylabel("L2 Error");
        // }
        // // Enable legend.
        // // matplotlibcpp::legend();
        // // Save the image (file format is determined by the extension)

        // try 
        // { 
        //     matplotlibcpp::save(save_title);
        // }
        // catch (const std::exception& e)
        // {
        //     std::cout << std::endl << e.what() << std::endl;
        //     std::cout << "Expects folder to exist: './optimisation-graphs'" << std::endl;
        // }
        
        // if(display_graph)
        // {
        //     matplotlibcpp::show();
        // }
    }

};

#endif