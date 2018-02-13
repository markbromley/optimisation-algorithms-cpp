#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <vector>

// Represent a single datapoint
class DataPoint
{
    private:
        
        std::vector<double> features;
        std::vector<double> targets;

    public:
        
        DataPoint(std::vector<double>& feat, std::vector<double>& tar)
        {
            features = feat;
            targets = tar;
        }

        // Get feature i
        double getFeature(unsigned int i)
        {
            return features.at(i);
        }

        // Set feature i
        void setFeature(unsigned int i, double val)
        {
            features[i] = val;
        }
        
        // Get all features
        std::vector<double>& getFeatures()
        {
            return features;
        }
        
        // Get target i
        double getTarget(unsigned int i)
        {
            return targets.at(i);
        }

        // Set target i
        void setTarget(unsigned int i, double val)
        {
            targets[i] = val;
        }
        
        // Get all targets
        std::vector<double>& getTargets()
        {
            return targets;
        }
};

#endif