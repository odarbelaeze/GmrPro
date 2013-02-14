#include "GmrPro.h"
#include <sstream>


class SystemAnalized : public System
{
public:
    SystemAnalized() : System(), distanceAcumulator_() {}
    SystemAnalized(const std::string fileName) : System(fileName), distanceAcumulator_() {}
    SystemAnalized(const char* fileName) : System(fileName), distanceAcumulator_() {}
    Vector getDistanceAcumulator() { return distanceAcumulator_; }
    void   resetDistanceAcumulator() { distanceAcumulator_ = Vector(); }

protected:
    virtual void  onDynamicEventCb_(Particle& particle, float energyDelta) {
        distanceAcumulator_ = distanceAcumulator_ + particle.getPosition() - particle.getOldPosition();
    }

private:
    Vector distanceAcumulator_;
};

int main(int argc, char const *argv[])
{
    Vector electricField;
    SystemAnalized system;
    srand48(time(NULL));

    try
    {    
        if (argc == 1)
            system = SystemAnalized("docs/descriptor_example.json");
        else
            system = SystemAnalized(argv[1]);
    }
    catch (BadDescriptorException& e)
    {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    system.addField("Electric", electricField);

    std::cout << "System ready, startng runs." << std::endl;

    Json::Reader reader;
    Json::Value  root;
    Json::Value  runs;
    std::ifstream runsFile;
    std::ofstream runsDataFile;
    char dataFileName[20];

    if (argc == 3)
        runsFile.open(argv[2]);
    else if (argc == 2)
        runsFile.open(argv[1]);
    else
        runsFile.open("docs/descriptor_example.json");

    bool parsingSuccessful = reader.parse(runsFile, root, false);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        throw BadDescriptorException("The descriptor is not a valid json file.");
    }
    runsFile.close();

    runs = root["runs"];
    int nRuns = runs.size();

    std::cout << "Ready to run " << nRuns << " simulations over the system."
              << std::endl;

    for (int i = 0; i < nRuns; ++i)
    {
        std::cout << "Runing " << i + 1 << " out of " << nRuns << "..."
                  << std::endl;
        
        std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
        std::ostringstream strCout;
        std::cout.rdbuf( strCout.rdbuf() );

        system.resetSystem();

        electricField = Vector(
            runs[i]["electricField"][0].asFloat(),
            runs[i]["electricField"][1].asFloat(),
            runs[i]["electricField"][2].asFloat()
        );

        system.setThermalEnergy(runs[i]["thermalEnergyDelta"].asFloat() * 
            runs[i]["multiplier"].asInt());

        for (int iii = 0; iii < runs[i]["multiplier"].asInt(); ++iii)
        {        
            for (int thermalStep = 0; thermalStep < runs[i]["thermalSteps"].asInt(); ++thermalStep)
                system.monteCarloThermalStep(false, false);
            for (int dynamicStep = 0; dynamicStep < runs[i]["dynamicSteps"].asInt(); ++dynamicStep)
                system.monteCarloDynamicStep(dynamicStep % 10 == 0);
            std::cout << system.getDistanceAcumulator() << std::endl;
            system.resetDistanceAcumulator();
            system.setThermalEnergy(system.getThermalEnergy() - 
                                    runs[i]["thermalEnergyDelta"].asFloat() );
        }

        std::cout.rdbuf( oldCoutStreamBuf );
        std::cout << "... OK" << std::endl;
        
        sprintf(dataFileName, "data/run%d", i + 1);
        runsDataFile.open(dataFileName);
        runsDataFile << strCout.str();
        runsDataFile.close();
    }

    return 0;
}