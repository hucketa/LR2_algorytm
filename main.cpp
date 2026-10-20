#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <climits>

class DataArray {
public:
    DataArray();
    DataArray(size_t arraySize);
    DataArray(const std::vector<int>& arrayData);
    void FillArray();
    void PrintArray();
    std::vector<int>& GetData();
private:
    size_t size;
    std::vector<int> data;
};

DataArray::DataArray() : size(0) {}

DataArray::DataArray(size_t arraySize) : size(arraySize) {
    data.resize(size, 0);
}

DataArray::DataArray(const std::vector<int>& arrayData) : size(arrayData.size()), data(arrayData) {}

void DataArray::FillArray() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
}

void DataArray::PrintArray() {
    const size_t elementsPerLine = 10;
    for (size_t i = 0; i < size; ++i) {
        if (i % elementsPerLine == 0) {
            std::cout << "[" << i << "]: ";
        }
        std::cout << std::setw(4) << data[i];
        if ((i + 1) % elementsPerLine == 0) {
            std::cout << '\n';
        }
    }
    if (size % elementsPerLine != 0) {
        std::cout << '\n';
    }
}

std::vector<int>& DataArray::GetData() {
    return data;
}

// Пошук мінімального елемента в масиві з StartIndex
size_t FindMinIndex(const std::vector<int>& data, size_t startIndex) {
    size_t minIndex = startIndex;
    for (size_t i = startIndex + 1; i < data.size(); ++i) {
        if (data[i] < data[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

// Сортування вибором
double SelectSort(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i + 1 < data.size(); ++i) {
        size_t minIndex = FindMinIndex(data, i);
        std::swap(data[i], data[minIndex]);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void PrintSortTime(const std::string& sortName, size_t size, double sortTimeUs) {
    std::cout << "\nАлгоритм сортування: " << sortName << '\n';
    std::cout << "Кількість елементів: " << size << '\n';
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Час виконання: " << sortTimeUs << " мкс\n";
}

// Двошляхове злиття
double TwoWayMerge(const std::vector<int>& first, const std::vector<int>& second, std::vector<int>& result) {
    auto start = std::chrono::steady_clock::now();
    result.resize(first.size() + second.size());
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    while (i < first.size() && j < second.size()) {
        if (first[i] <= second[j]) {
            result[k++] = first[i++];
        } else {
            result[k++] = second[j++];
        }
    }
    while (i < first.size()) {
        result[k++] = first[i++];
    }
    while (j < second.size()) {
        result[k++] = second[j++];
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}


// 1 в 1 Методичка
int Partition(std::vector<int>& data, int leftindex, int rightindex) {
    int leftScanIndex = leftindex - 1;
    int rightScanIndex = rightindex;
    int pivot = data[rightindex];
    while (true) {
        while (data[++leftScanIndex] < pivot);
        while (pivot < data[--rightScanIndex]) {
            if (rightScanIndex == leftindex) break;
        }
        if (leftScanIndex >= rightScanIndex) break;
        std::swap(data[leftScanIndex], data[rightScanIndex]);
    }
    std::swap(data[leftScanIndex], data[rightindex]);
    return leftScanIndex;
}

void QuickSortRecursive(std::vector<int>& data, int leftindex, int rightindex) {
    if (rightindex <= leftindex) return;
    int pivotIndex = Partition(data, leftindex, rightindex);
    QuickSortRecursive(data, leftindex, pivotIndex - 1);
    QuickSortRecursive(data, pivotIndex + 1, rightindex);
}

double QuickSort(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    if (!data.empty()) {
        QuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}


/*void LogArray(const std::vector<int>& data, int left, int right) {
    std::cout << "[";
    for (int i = left; i <= right; ++i) {
        if (i > left) std::cout << " ";
        std::cout << data[i];
    }
    std::cout << "]\n";
}

// Копія з методички
int PartitionLogged(std::vector<int>& data, int leftindex, int rightindex) {
    int leftScanIndex = leftindex - 1;
    int rightScanIndex = rightindex;
    int pivot = data[rightindex];
    std::cout << "\nPARTITION [" << leftindex << ", " << rightindex << "]\n";
    std::cout << "Pivot: " << pivot << " (index " << rightindex << ")\n";
    std::cout << "Before: ";
    LogArray(data, leftindex, rightindex);
    while (true) {
        while (data[++leftScanIndex] < pivot) {
            std::cout << "  Left scan: index " << leftScanIndex << ", value " << data[leftScanIndex] << '\n';
        }
        std::cout << "  Left stopped: index " << leftScanIndex << ", value " << data[leftScanIndex] << '\n';
        while (pivot < data[--rightScanIndex]) {
            std::cout << "  Right scan: index " << rightScanIndex << ", value " << data[rightScanIndex] << '\n';
            if (rightScanIndex == leftindex) break;
        }
        std::cout << "  Right stopped: index " << rightScanIndex << ", value " << data[rightScanIndex] << '\n';
        if (leftScanIndex >= rightScanIndex) {
            std::cout << "  Pointers crossed.\n";
            break;
        }
        std::cout << "  Swap: " << data[leftScanIndex] << " <-> " << data[rightScanIndex] << '\n';
        std::swap(data[leftScanIndex], data[rightScanIndex]);
        std::cout << "  After swap: ";
        LogArray(data, leftindex, rightindex);
    }
    std::cout << "Place pivot at index " << leftScanIndex << '\n';
    std::swap(data[leftScanIndex], data[rightindex]);
    std::cout << "After partition: ";
    LogArray(data, leftindex, rightindex);
    return leftScanIndex;
}

void QuickSortRecursiveLogged(std::vector<int>& data, int leftindex, int rightindex) {
    if (rightindex <= leftindex) {
        std::cout << "Range [" << leftindex << ", " << rightindex << "] - no sorting needed.\n";
        return;
    }
    std::cout << "\nQUICKSORT [" << leftindex << ", " << rightindex << "]\n";
    std::cout << "Current array: ";
    LogArray(data, leftindex, rightindex);
    int pivotIndex = PartitionLogged(data, leftindex, rightindex);
    std::cout << "Left recursion: [" << leftindex << ", " << pivotIndex - 1 << "]\n";
    QuickSortRecursiveLogged(data, leftindex, pivotIndex - 1);
    std::cout << "Right recursion: [" << pivotIndex + 1 << ", " << rightindex << "]\n";
    QuickSortRecursiveLogged(data, pivotIndex + 1, rightindex);
}

double QuickSortLogged(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    if (!data.empty()) {
        QuickSortRecursiveLogged(data, 0, static_cast<int>(data.size()) - 1);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

// Кінець копії з методички*/

// MSD

const int RADIX = 10;

void MSDRecursive(std::vector<int>& data, std::vector<int>& auxiliary, int left, int right, int divisor) {
    if (left >= right || divisor <= 0) return;
    int count[RADIX + 1] = {};
    for (int currentIndex = left; currentIndex <= right; ++currentIndex) {
        int digit = (data[currentIndex] / divisor) % RADIX;
        count[digit + 1]++;
    }
    for (int bucketIndex = 1; bucketIndex <= RADIX; ++bucketIndex) {
        count[bucketIndex] += count[bucketIndex - 1];
    }
    int bucketStart[RADIX];
    int bucketEnd[RADIX];
    for (int bucketIndex = 0; bucketIndex < RADIX; ++bucketIndex) {
        bucketStart[bucketIndex] = left + count[bucketIndex];
        bucketEnd[bucketIndex] = left + count[bucketIndex + 1] - 1;
    }
    for (int currentIndex = left; currentIndex <= right; ++currentIndex) {
        int digit = (data[currentIndex] / divisor) % RADIX;
        auxiliary[left + count[digit]++] = data[currentIndex];
    }
    for (int currentIndex = left; currentIndex <= right; ++currentIndex) {
        data[currentIndex] = auxiliary[currentIndex];
    }
    for (int bucketIndex = 0; bucketIndex < RADIX; ++bucketIndex) {
        if (bucketStart[bucketIndex] < bucketEnd[bucketIndex]) {
            MSDRecursive(data, auxiliary, bucketStart[bucketIndex], bucketEnd[bucketIndex], divisor / RADIX);
        }
    }
}

double MSDSort(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    if (!data.empty()) {
        int maxValue = *std::max_element(data.begin(), data.end());
        int divisor = 1;
        while (maxValue / divisor >= RADIX) divisor *= RADIX;
        std::vector<int> auxiliary(data.size());
        MSDRecursive(data, auxiliary, 0, static_cast<int>(data.size()) - 1, divisor);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    const std::vector<size_t> arraySizes = {
        50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600
    };
    std::cout << std::fixed << std::setprecision(2);
    std::vector<std::vector<double>> results;
    for (size_t arraySize : arraySizes) {
        DataArray sourceArray(arraySize);
        sourceArray.FillArray();
        std::vector<int> originalData = sourceArray.GetData();
        std::cout << "\n========== Розмір масиву: " << arraySize << " ==========\n";
        std::vector<int> selectionData = originalData;
        std::cout << "\n1) Сортування вибором\n";
        std::cout << "Початковий масив:\n";
        sourceArray.PrintArray();
        double selectionTime = SelectSort(selectionData);
        std::cout << "Результат:\n";
        DataArray selectionResult(selectionData);
        selectionResult.PrintArray();
        std::vector<int> firstHalf(originalData.begin(), originalData.begin() + originalData.size() / 2);
        std::vector<int> secondHalf(originalData.begin() + originalData.size() / 2, originalData.end());
        std::cout << "\n2) Двошляхове злиття\n";
        std::cout << "Початковий масив 1:\n";
        DataArray firstArray(firstHalf);
        firstArray.PrintArray();
        std::cout << "Початковий масив 2:\n";
        DataArray secondArray(secondHalf);
        secondArray.PrintArray();
        std::sort(firstHalf.begin(), firstHalf.end());
        std::sort(secondHalf.begin(), secondHalf.end());
        std::vector<int> mergedData;
        double mergeTime = TwoWayMerge(firstHalf, secondHalf, mergedData);
        std::cout << "Результат злиття:\n";
        DataArray mergeResult(mergedData);
        mergeResult.PrintArray();
        std::vector<int> quickData = originalData;
        std::cout << "\n3) Швидке сортування (Quick Sort)\n";
        std::cout << "Початковий масив:\n";
        sourceArray.PrintArray();
        double quickTime = QuickSort(quickData);
        std::cout << "Результат:\n";
        DataArray quickResult(quickData);
        quickResult.PrintArray();
        std::vector<int> msdData = originalData;
        std::cout << "\n4) Сортування MSD\n";
        std::cout << "Початковий масив:\n";
        sourceArray.PrintArray();
        double msdTime = MSDSort(msdData);
        std::cout << "Результат:\n";
        DataArray msdResult(msdData);
        msdResult.PrintArray();
        results.push_back({selectionTime, mergeTime, quickTime, msdTime});
    }
    std::cout << "\n========== Таблиця часу виконання ==========\n";
    std::cout << "Розмір | Сортування вибором | Двошляхове злиття | Quick Sort | MSD\n";
    std::cout << "-------|--------------------|-------------------|------------|----------\n";
    for (size_t index = 0; index < arraySizes.size(); ++index) {
        std::cout << std::setw(6) << arraySizes[index] << " | "
                  << std::setw(18) << results[index][0] << " | "
                  << std::setw(17) << results[index][1] << " | "
                  << std::setw(10) << results[index][2] << " | "
                  << std::setw(8) << results[index][3] << '\n';
    }
    std::cout << "\nЧас виконання вимірюється в мікросекундах.\n";
    system("pause");
    return 0;
}