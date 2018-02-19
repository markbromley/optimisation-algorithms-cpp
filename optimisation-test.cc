#include <iostream>
#include <fstream>
#include <vector>

#include "lib/datapoint.h"
#include "lib/gradient.h"
#include "lib/normalisation.h"


using namespace std;

std::ostream& operator<<(std::ostream& os, DataPoint& data_point)
{
    int number_of_features = data_point.getFeatures().size();
    os << "(";
    for (int i = 0; i < number_of_features; i++)
    {
        os << data_point.getFeature(i) << (i != number_of_features-1 ? ", " : ""); 
    }
    os << ")";
    return os;
}

// Runs an implementation of multiple linear regression, using the gradient descent
// implementation
int main(int argc, char **argv )
{
    try
    {
        // Get the config params
        ConfigParameters parameters( argv[1] );
        for( int i = 2; i < argc; ++i )
        {
            parameters.add( argv[ i ] );
        }  

        std::shared_ptr<ConfigParameters> params = std::make_shared<ConfigParameters>(parameters);
    }
    catch( const std::string& e )
    {
        std::cout << "Exception: " << e << std::endl;
        return 1;
    }
    catch( const char* e )
    {
        std::cout << "Exception: " << e << std::endl;
        return 1;
    }
    catch( std::exception& e )
    {
        std::cerr << "exception caught: " << e.what() << std::endl;
        return 1;
    }







    unsigned int number_of_data_points = 0;
    unsigned int number_of_features = 0;
    vector<DataPoint> examples;
    std::ifstream file;
    
    // Open the test data file
    file.open("./lib/data.txt");
    if (!file.is_open())
    {
        std::cout << "File not opened." << std::endl;
        return 1;
    }

    // Read in the file
    file >> number_of_data_points >> number_of_features;
    std::cout << "# of Data Points: " << number_of_data_points << 
        ", # of Features: " << number_of_features << std::endl;


    for (unsigned int i = 0; i < number_of_data_points; i++)
    {
        // Add in the features (plus extra column for intercept feature)
        std::vector<double> feat(number_of_features+1, 1);
        for (unsigned int j = 0; j < number_of_features; j++)
        {
            file >> feat[j+1];
        }

        // Add in the target values
        std::vector<double> target(1,0);
        file >> target[0];

        DataPoint data_point(feat, target);
        examples.push_back(data_point);
    }
    file.close();

    // Print each example
    for (unsigned int i = 0; i < examples.size(); i++)
    {
        std::cout << "Example " << i << ": " << examples[i] << std::endl;
    }

    // Get the min/ max for the dataset
    std::pair<double, double> minMaxPair = Normalisation::getMinMaxFromAllData(examples);
    // Set the intial theta values to 1.0 (terrible idea!)
    auto intial_theta = std::vector<double>(examples[0].getFeatures().size(), 1.0);
    // Normalise    
    std::vector<DataPoint> normalised_training_examples = Normalisation::normaliseAllData(
        examples, minMaxPair.first, minMaxPair.second);
    
    // Run the SGD algorithm
    GradientDescent grad(normalised_training_examples, intial_theta, minMaxPair);
    grad.printData();
    std::vector<double> theta = grad.run();

    // Print the results
    std::cout << std::endl;
    for (unsigned int i = 0; i < theta.size(); i++)
    {
        std::cout << "Theta_" << i << ": " << theta[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}// end main

