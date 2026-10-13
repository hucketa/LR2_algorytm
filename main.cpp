#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <algorithm>
#include <chrono>

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

DataArray::DataArray(const std::vector<int>& arrayData)
    : size(arrayData.size()), data(arrayData) {}

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

//Пошук мінімального елемнта в масиві з StartIndex
size_t FindMinIndex(const std::vector<int>& data, size_t startIndex) {
    size_t minIndex = startIndex;
    for (size_t i = startIndex + 1; i < data.size(); ++i) {
        if (data[i] < data[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

//Сортування вибором
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

//Двошляхове злиття
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




int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    /*
          // Сортування вибором
    size_t n;
    std::cout << "Введіть кількість елементів масиву для сортування вибором:\n";
    std::cin >> n;
    DataArray selectionArray(n);
    selectionArray.FillArray();
    std::cout << "\nПочатковий масив для сортування вибором:\n";
    selectionArray.PrintArray();
    double selectionTimeUs = SelectSort(selectionArray.GetData());
    std::cout << "\nРезультат сортування вибором:\n";
    selectionArray.PrintArray();
    PrintSortTime("Сортування вибором (Selection Sort)", n, selectionTimeUs);

    // Двошляхове злиття
    size_t n1;
    size_t n2;
    std::cout << "\nВведіть кількість елементів першого масиву для двошляхового злиття:\n";
    std::cin >> n1;
    std::cout << "Введіть кількість елементів другого масиву для двошляхового злиття:\n";
    std::cin >> n2;
    DataArray first(n1);
    DataArray second(n2);
    first.FillArray();
    second.FillArray();

    // Сортування масивів перед злиттям
    std::sort(first.GetData().begin(), first.GetData().end());
    std::sort(second.GetData().begin(), second.GetData().end());
    std::cout << "\nПерший відсортований масив:\n";
    first.PrintArray();
    std::cout << "\nДругий відсортований масив:\n";
    second.PrintArray();

    //Двошляхове злиття
    std::vector<int> result;
    double mergeTimeUs = TwoWayMerge(first.GetData(), second.GetData(), result);
    DataArray merged(result.size());
    merged.GetData() = result;
    std::cout << "\nРезультат двошляхового злиття:\n";
    merged.PrintArray();
    PrintSortTime("Двошляхове злиття (Two-Way Merge)", result.size(), mergeTimeUs);
    */

    

    // Тестування сортування вибором
    std::vector<std::vector<int>> selectionTests = {
        {29, 8, 86, 87, 5, 34},
        {1, 2, 3, 4, 5, 6},
        {6, 5, 4, 3, 2, 1},
        {5, 5, 5, 5, 5},
        {3, 1, 3, 2, 3, 1, 2},
        {-10, 0, 7, -3, 2, -10},
        {42},
        {}
    };
    for (size_t testIndex = 0; testIndex < selectionTests.size(); ++testIndex) {
        DataArray testArray(selectionTests[testIndex]);
        std::vector<int> expected = selectionTests[testIndex];
        std::sort(expected.begin(), expected.end());
        std::cout << "\n========== SELECTION SORT: ТЕСТ " << testIndex + 1 << " ==========\n";
        std::cout << "Початковий масив:\n";
        testArray.PrintArray();
        double sortTimeUs = SelectSort(testArray.GetData());
        std::cout << "Результат сортування:\n";
        testArray.PrintArray();
        bool isCorrect = std::is_sorted(testArray.GetData().begin(), testArray.GetData().end())
            && testArray.GetData() == expected;
        std::cout << "Перевірка результату: " << (isCorrect ? "УСПІШНО" : "ПОМИЛКА") << '\n';
        PrintSortTime("Selection Sort", testArray.GetData().size(), sortTimeUs);
    }
    // Тестування двошляхового злиття
    std::vector<std::pair<std::vector<int>, std::vector<int>>> mergeTests = {
        {{1, 3, 5}, {2, 4, 6}},
        {{1, 2, 3}, {4, 5, 6}},
        {{4, 5, 6}, {1, 2, 3}},
        {{2, 2, 5}, {2, 3, 5}},
        {{-10, -3, 2}, {-5, 0, 7}},
        {{}, {1, 2, 3}},
        {{1, 2, 3}, {}},
        {{}, {}},
        {{42}, {42}},
        {{1, 1, 1}, {1, 1}}
    };
    for (size_t testIndex = 0; testIndex < mergeTests.size(); ++testIndex) {
        DataArray first(mergeTests[testIndex].first);
        DataArray second(mergeTests[testIndex].second);
        std::vector<int> expected = mergeTests[testIndex].first;
        expected.insert(expected.end(), mergeTests[testIndex].second.begin(), mergeTests[testIndex].second.end());
        std::sort(expected.begin(), expected.end());
        std::vector<int> result;
        std::cout << "\n========== TWO-WAY MERGE: ТЕСТ " << testIndex + 1 << " ==========\n";
        std::cout << "Перший масив:\n";
        first.PrintArray();
        std::cout << "Другий масив:\n";
        second.PrintArray();
        double mergeTimeUs = TwoWayMerge(first.GetData(), second.GetData(), result);
        DataArray merged(result);
        std::cout << "Результат злиття:\n";
        merged.PrintArray();
        bool isCorrect = std::is_sorted(result.begin(), result.end())
            && result == expected;
        std::cout << "Перевірка результату: " << (isCorrect ? "УСПІШНО" : "ПОМИЛКА") << '\n';
        PrintSortTime("Two-Way Merge", result.size(), mergeTimeUs);
    }
    return 0;

}