#include<iostream>
#include<iomanip>
#include<string>

using namespace std;
//Constant values in the functions

int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
int P4[4] = {2, 4, 3, 1};
int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_inverse[8] = {4, 1, 3, 5, 7, 2, 8, 6};
int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1 };
int s0[4][4] =  {
                {1, 0, 3, 2},
                {3, 2, 1, 0},
                {0, 2, 1, 3},
                {3, 1, 3, 2}};
int s1[4][4] =  {
                {0, 1, 2, 3},
                {2, 0, 1, 3},
                {3, 0, 1, 0},
                {2, 1, 0, 3}};
int PlainTextSize = 8;
int bigKeySize = 10;
int smallKeySize = 8;

// Funstion Declaration
int twoTypeInput(int a, int b);
void printBinaryText(int size, int* arrayPointer);
void inputBinaryText(int size, int* arrayPointer);
void inputAndOutputBinaryText(int size, int* arrayPointer, string inputName);
void applyPermutation(int sizeOfPermutation, int* resultPointer, int PointerKey[], int* toApplyOn);
void leftShiftOne(int* result, int* toShift, int toShiftSize);
void leftShiftTwo(int* result, int* toShift, int toShiftSize);
void devideBits(int* left, int* right, int*toDevide, int toDevideSize);
void combineBits(int* array1, int* array2, int* result, int size);
void addTwoBinary(int array1[], int array2[], int result[], int size);
int calculateValue(int a[]);
void reverseValue(int value, int* result);
void applySBox(int* key_EP, int* S_Result);
void Box(int* RightBitInput, int* LeftBitInput,int* key, int* BitOutPut);
void getKeys(int* binaryKey, int* resultKeyOne, int* resultKeyTwo);


int main(){ 

    cout << endl << "What would you like to do?" << endl;
    cout << "Choose 1 for encryption" << endl;
    cout << "Choose 2 for decryption" << endl;
    int choose = 0;
    choose = twoTypeInput(1, 2);

    // Encryption
    if(choose == 1)
    {
        int* plainText = new int [PlainTextSize];
        int* tenDigitKey = new int [bigKeySize];
        
        // Taking in Plain Text and 10 bits key
        inputAndOutputBinaryText(PlainTextSize, plainText, "PlainText");
        inputAndOutputBinaryText(bigKeySize, tenDigitKey, "Binary Key");

        cout << "Plain Text:       ";
        printBinaryText(PlainTextSize, plainText);
        cout << "Binary Key:       ";
        printBinaryText(PlainTextSize, tenDigitKey);

        // Get encryption Key
        int* key_one = new int [smallKeySize];
        int* key_two = new int [smallKeySize];
        getKeys(tenDigitKey, key_one, key_two);


        // Encrypting Plain Text -----------------Start here------------------//
        // Apply IP on PlainText
        int* IP_PlainText = new int [PlainTextSize];
        cout << "IP(Plain Text):   ";
        applyPermutation(PlainTextSize, IP_PlainText, IP, plainText);

        //Store Left Bits and Right Bits
        int* Left_Bits = new int [PlainTextSize/2];
        int* Right_Bits = new int [PlainTextSize/2];
        devideBits(Left_Bits, Right_Bits, IP_PlainText, PlainTextSize/2);
        cout << "Left Bits:        ";
        printBinaryText(PlainTextSize/2, Left_Bits);
        cout << "Right Bits:       ";
        printBinaryText(PlainTextSize/2, Right_Bits);

        //Right 4 bits going in the box
        int* Box_One_Output = new int[4];
        Box(Right_Bits, Left_Bits, key_one, Box_One_Output);
        //Result is P4 + Left Bits

        //Swap, Right bits become left, left bits become right
        Left_Bits = Right_Bits; 
        Right_Bits = Box_One_Output;
        int* Box_Two_Output = new int[4];
        Box(Right_Bits, Left_Bits, key_two, Box_Two_Output);

        int* eight_bit_combined = new int[PlainTextSize];
        combineBits(Box_Two_Output, Right_Bits, eight_bit_combined,4);

        //Apply IP Inverse to the previous result
        int* cipherText = new int[PlainTextSize];
        cout << "Cipher Text:      ";
        applyPermutation(PlainTextSize, cipherText, IP_inverse, eight_bit_combined);

        //Deleting waste data
        delete[] plainText, tenDigitKey, key_one, key_two, IP_PlainText, Left_Bits, Right_Bits, Box_One_Output, Box_Two_Output, eight_bit_combined, cipherText;
    }
    //Decryption
    else if (choose == 2){
        // Decryption
    int* cipherText = new int[PlainTextSize];
    int* tenDigitKey = new int[bigKeySize];

    // Taking in Cipher Text and 10-bit key (same as encryption)
    inputAndOutputBinaryText(PlainTextSize, cipherText, "Cipher Text");
    inputAndOutputBinaryText(bigKeySize, tenDigitKey, "Binary Key");

    cout << "Cipher Text:      ";
    printBinaryText(PlainTextSize, cipherText);
    cout << "Binary Key:       ";
    printBinaryText(PlainTextSize, tenDigitKey);
    
    // Get encryption Key
    int* key_one = new int [smallKeySize];
    int* key_two = new int [smallKeySize];
    getKeys(tenDigitKey, key_one, key_two);

    // Decrypting Cipher Text -----------------Start here------------------//
    // Apply IP on PlainText
    int* IP_CipherText = new int [PlainTextSize];
    cout << "IP(Cipher Text):  ";
    applyPermutation(PlainTextSize, IP_CipherText, IP, cipherText);

    // Store Left Bits and Right Bits
    int* Left_Bits = new int [PlainTextSize/2];
    int* Right_Bits = new int [PlainTextSize/2];
    devideBits(Left_Bits, Right_Bits, IP_CipherText, PlainTextSize/2);
    cout << "Left Bits:        ";
    printBinaryText(PlainTextSize/2, Left_Bits);
    cout << "Right Bits:       ";
    printBinaryText(PlainTextSize/2, Right_Bits);

    // Right 4 bits going in the box with key_two
    int* Box_One_Output = new int[4];
    Box(Right_Bits, Left_Bits, key_two, Box_One_Output);

    // Swap, Right bits become left, left bits become right
    Left_Bits = Right_Bits;
    Right_Bits = Box_One_Output;

    int* Box_Two_Output = new int[4];
    Box(Right_Bits, Left_Bits, key_one, Box_Two_Output);

    int* eight_bit_combined = new int[PlainTextSize];
    combineBits(Box_Two_Output, Right_Bits, eight_bit_combined, 4);

    // Apply IP Inverse to the previous result
    int* decryptedText = new int[PlainTextSize];
    cout << "Decrypted Text:   ";
    applyPermutation(PlainTextSize, decryptedText, IP_inverse, eight_bit_combined);

    // Deleting waste data
    delete[] cipherText, tenDigitKey, key_one, key_two, IP_CipherText, Left_Bits, Right_Bits, Box_One_Output, Box_Two_Output, eight_bit_combined, decryptedText;

    }


    return 0;
}

int twoTypeInput(int a, int b){
    int choice;
    do{
        cin >> choice;
        if(choice !=a && choice !=b){
            cout << "Please make a selection between " << a << " and " << b << endl;
        }
    }
    while(choice !=a && choice !=b);
    
    return choice;
}

void printBinaryText(int size, int* arrayPointer){
    for(int i=0; i< size; i++){
        cout << arrayPointer[i] << " ";
    }
    cout << endl;
}

void inputBinaryText(int size, int* arrayPointer){
    cout << "Please enter 1 digit at a time. Enter either 1 or 0 " << endl;
    for(int i = 0; i < size; i++){
        cout << "Choose digit number " << i+1 << endl;
        arrayPointer[i] = twoTypeInput(0, 1);
    }
}

void inputAndOutputBinaryText(int size, int* arrayPointer, string inputName){
    
    cout << endl << "Please enter " << size << " digit Binary " << inputName << endl;
    inputBinaryText(size, arrayPointer);
    cout << inputName << ": ";
    printBinaryText(size, arrayPointer);
    cout << endl;
}

void applyPermutation(int sizeOfPermutation, int* resultPointer, int PointerKey[], int* toApplyOn){
    for(int i = 0; i < sizeOfPermutation; i++){
        resultPointer[i] = toApplyOn[PointerKey[i]-1];
    }
    printBinaryText(sizeOfPermutation, resultPointer);
}

void leftShiftOne(int* result, int* toShift, int toShiftSize){
    for(int i=0; i < toShiftSize/2; i++){
        if(i < toShiftSize-1){
            result[i] = toShift[i+1];
            result[(toShiftSize/2)+i] = toShift[i+1+(toShiftSize/2)];
        }
        if(i == toShiftSize/2 - 1){
            result[i] = toShift[0];
            result[toShiftSize-1] = toShift[i+1];
        }
    }
    printBinaryText(toShiftSize, result);
}

void leftShiftTwo(int* result, int* toShift, int toShiftSize){

    for(int i=0; i < toShiftSize/2; i++){
        if(i < toShiftSize-2){
            result[i] = toShift[i+2];
            result[(toShiftSize/2)+i] = toShift[i+2+(toShiftSize/2)];
        }
        if(i == toShiftSize/2 - 2){
            result[i] = toShift[0];
            result[toShiftSize-2] = toShift[i+2];
        }
        if(i == toShiftSize/2 - 1){
            result[i] = toShift[1];
            result[toShiftSize-1] = toShift[i+2];
        }
    }
    printBinaryText(toShiftSize, result);
}

void addTwoBinary(int array1[], int array2[], int result[], int size){
    for (int i = 0; i < size; i++) {
        if (array1[i] == 0 && array2[i] == 0) {
            result[i] = 0;
        } 
        else if (array1[i] == 1 && array2[i] == 1) {
            result[i] = 0;
        } 
        else if (array1[i] == 0 && array2[i] == 1) {
            result[i] = 1;
        } 
        else if (array1[i] == 1 && array2[i] == 0) {
            result[i] = 1;
        } 
    }
}

void devideBits(int* left, int* right, int*toDevide, int toDevideSize){
    for(int i = 0; i < toDevideSize; i++){
            left[i] = toDevide[i];
            right[i]= toDevide[i+toDevideSize];
    }
}

void combineBits(int* array1, int* array2, int* result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = array1[i];
        result[i + size] = array2[i];
    }
}

void applySBox(int* key_EP, int* S_Result) {
    int Row1[2] = {key_EP[0], key_EP[3]};
    int Column1[2] = {key_EP[1], key_EP[2]};
    int Row2[2] = {key_EP[4], key_EP[7]};
    int Column2[2] = {key_EP[5], key_EP[6]};

    int S_result_one = s0[calculateValue(Row1)][calculateValue(Column1)];
    int S_result_two = s1[calculateValue(Row2)][calculateValue(Column2)];
    // Calculate value function get a Row and column number. 
    //Then s0 and s1 are extracted of a decimal
    reverseValue(S_result_one, &S_Result[0]);
    reverseValue(S_result_two, &S_Result[2]);
}

int calculateValue(int a[]){
    if(a[0] == 0 && a[1] == 0){
        return 0;
    }
    else if(a[0] == 0 && a[1] == 1){
        return 1;
    }
    else if(a[0] == 1 && a[1] == 0){
        return 2;
    }
    else if(a[0] == 1 && a[1] == 1){
        return 3;
    }
    return 0;
}

void reverseValue(int value, int* result) {
    
    switch (value) {
        case 0:
            result[0] = 0;
            result[1] = 0;
            break;
        case 1:
            result[0] = 0;
            result[1] = 1;
            break;
        case 2:
            result[0] = 1;
            result[1] = 0;
            break;
        case 3:
            result[0] = 1;
            result[1] = 1;
            break;
    }
}

void Box(int* RightBitInput, int* LeftBitInput,int* key, int* BitOutPut){
           
        int* Applied_EP = new int[PlainTextSize];
        cout << "E/P:              ";
        applyPermutation(PlainTextSize, Applied_EP, EP, RightBitInput);

        // Performing E/P + Key 1
        int* key_EP = new int[PlainTextSize];
        addTwoBinary(Applied_EP, key, key_EP, PlainTextSize);
        cout << "E/P + Key 1:      ";
        printBinaryText(PlainTextSize, key_EP);

        // Extracting bits for S-box looku
        int* S_Result = new int[4];
        applySBox(key_EP, S_Result);
        cout << "After Appling S:  ";
        printBinaryText(PlainTextSize/2, S_Result);

        //Apply P4 on previous result
        int* P4_Result = new int [PlainTextSize/2];
        cout << "Apply P4 on S:    ";
        applyPermutation(PlainTextSize/2, P4_Result, P4, S_Result);

        //P4 result + Left Bits
        addTwoBinary(P4_Result, LeftBitInput, BitOutPut, 4);
        cout << "P4 + Left Bits:   ";
        printBinaryText(4, BitOutPut);

        delete[] Applied_EP, key_EP, S_Result, P4_Result;
}

void getKeys(int* binaryKey, int* resultKeyOne, int* resultKeyTwo){

    //Apply P10 on 10 digit key
    int* P10Result = new int[bigKeySize];
    cout << "Key after P10:    ";
    applyPermutation(bigKeySize, P10Result, P10, binaryKey);
        
    //Left Shift by 1 bit
    int* leftShift1 = new int [bigKeySize];
    cout << "Left shift 1:     ";
    leftShiftOne(leftShift1, P10Result, bigKeySize);

    //Left Shift by 2 bit
    int* leftShift2 = new int [bigKeySize];
    cout << "Left shift 2:     ";
    leftShiftTwo(leftShift2, leftShift1, bigKeySize);

    //Apply P8 on Result of Left SHift 1
    cout << "Calculated Key 1: ";
    applyPermutation(smallKeySize, resultKeyOne, P8, leftShift1);

    //Apply P8 on Result of Left Shift 2
    cout << "Calculated Key 2: ";
    applyPermutation(smallKeySize, resultKeyTwo, P8, leftShift2);

    delete[] P10Result, leftShift1, leftShift2;
}


