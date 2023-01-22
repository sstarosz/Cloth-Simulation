

Some function may return c++ container with a invalid size for example


const auto memoryProperties = device.getMemoryProperties2();
const auto memoryHeaps = memoryProperties.memoryProperties.memoryHeaps;
const auto memoryHeapsCount = memoryProperties.memoryProperties.memoryHeapCount;

memoryHeaps is a std::array with size 32
memoryHeapCount is size of 5 on my GPU