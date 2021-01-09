#include "sys/times.h"
#include "sys/vtimes.h"

class CPUAndMemoryConsumption
{
        clock_t lastCPU, lastSysCPU, lastUserCPU;
        int numProcessors;
    public:
        int test = 0;
        // MAIN FUNCTIONAL METHODS:
        /** 
         * @return Returns the percentage of CPU which the current process is currently using.
         * Requires the method CPUPercentageUsageInit() to be called at the beginning of the program.
         * */
        double CPUPercentageUsage();
        /** 
         * @brief This method has to be called at the beginning of the program in order
         * to for the method CPUPercentageUsage() to work properly.
         * */
        void CPUPercentageUsageInit();
        /** 
         * @return Returns the physical (that is RAM) memory (in MB) currently used by the current process.
         * */
        double physicalMemory();
        /** 
         * @return Returns the virtual memory (in MB) currently used by the current process.
         * */
        double virtualMemory();

        // LEVEL 0 METHODS:
        int parseLine(char* line);
}; 