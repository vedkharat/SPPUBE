#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int size;
    cout<<"Enter size of arr: ";
    cin>>size;
    int arr[size];
    for(int i=0;i<size;i++){
        cout<<"Enter elements: ";
        cin>>arr[i];
    }
    for(int i=0;i<size;i++){
        cout<<arr[i];
    }
    // vector<int> arr = {5, 3, 8, 4, 2, 7, 1, 6}; // Sample array
    int minVal = arr[0], maxVal = arr[0], sum = 0;
    double avg;
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < size; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
        sum += arr[i];
    }
    avg = sum /size;
    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
    return 0;
}
