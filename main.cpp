#include "CPUAndMemoryConsumption.h"

#include <vector>
#include <stdio.h>

using namespace std;

int main()
{
    CPUAndMemoryConsumption cpuAndMemoryConsumption;

    cpuAndMemoryConsumption.CPUPercentageUsageInit();

    printf("At the startup: %.2lf percent (CPU)\t %.2lf MB (physical memory)\t %.2lf MB (virtual memory)\n", 
            cpuAndMemoryConsumption.CPUPercentageUsage(), cpuAndMemoryConsumption.physicalMemory(), cpuAndMemoryConsumption.virtualMemory());

    int vectorSize = 10000000;
    vector<int> V(vectorSize);
    for(int i=0;i<vectorSize;i++)
    {
        V[i] = i;
        if(i == vectorSize/2)
        {
            printf("In the middle: %.2lf percent (CPU)\t %.2lf MB (physical memory)\t %.2lf MB (virtual memory)\n", 
                    cpuAndMemoryConsumption.CPUPercentageUsage(), cpuAndMemoryConsumption.physicalMemory(), cpuAndMemoryConsumption.virtualMemory());
        }
    }
    V.clear();
    printf("At the end: %.2lf percent (CPU)\t %.2lf MB (physical memory)\t %.2lf MB (virtual memory)\n", 
            cpuAndMemoryConsumption.CPUPercentageUsage(), cpuAndMemoryConsumption.physicalMemory(), cpuAndMemoryConsumption.virtualMemory());
    return 0;
}