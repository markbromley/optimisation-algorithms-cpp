#ifndef NORMALISATION_H
#define NORMALISATION_H

#include <vector>
#include <limits>
#include <utility>

#include "datapoint.h"

class Normalisation
{

public:

    // Returns the min/ max from all the data set
    static std::pair<double, double> getMinMaxFromAllData(std::vector<DataPoint> examples)
    {
        // Includes artificial feature (constant of 1)
        auto num_features = examples[0].getFeatures().size();
        auto num_targets = examples[0].getTargets().size();

        std::cout << "Total number of features: " <<  num_features << std::endl;
        
        // Get the min/max for target
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::min();

        for(auto ex: examples)
        {
            for(unsigned int i = 0; i < num_targets; i++)
            {            
                auto result = Normalisation::getMinMaxFromTrainingExample(ex.getTarget(i), 
                    min, max);
                min = result.first;
                max = result.second;
            }

            // Skip first column, as constant
            for(unsigned int i = 0; i < num_features; i++)
            {
                auto result = Normalisation::getMinMaxFromTrainingExample(ex.getFeature(i), 
                    min, max);
                min = result.first;
                max = result.second;
            }

        }

        // Don't use the edge of the scales, as they may include 0s
        min -= 1;
        max += 1;
        return std::make_pair(min, max);
    }

    // Returns the normalised representation of a data set.
    static std::vector<DataPoint> normaliseAllData(std::vector<DataPoint> examples, double min, double max)
    {
        // Includes artificial feature (constant of 1)
        auto num_features = examples[0].getFeatures().size();
        auto num_targets = examples[0].getTargets().size();

        // Normalise all the data points
        for(auto&& ex: examples)
        {
            for(unsigned int i = 0; i < num_features; i++)
            {
                double new_feat_val = Normalisation::normaliseDataPoint(ex.getFeature(i),
                                                                        max,
                                                                        min);
                ex.setFeature(i, new_feat_val);
            }
            
            for(unsigned int i = 0; i < num_targets; i++)
            {            
                double new_target_val = Normalisation::normaliseDataPoint(ex.getTarget(i),
                                                                          max,
                                                                          min);
                ex.setTarget(i, new_target_val);
            }
        }
        return examples;
    }

    // Gets the min/ max from one example
    static std::pair<double, double> getMinMaxFromTrainingExample(double val, double min, double max)
    {
        min = ((min > val) ? val : min);
        max = ((max < val) ? val : max);
        return std::make_pair(min, max);
    }

    // Normalises a single datapoint
    static double normaliseDataPoint(double data_point, double max, double min)
    {
        return ((data_point - min) / (max - min));
    }

private:

};

#endif