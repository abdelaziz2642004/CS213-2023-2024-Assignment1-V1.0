#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstring>
#include "bmplib.cpp"
#include <cmath>
#include <vector>
// updated by Abdleaziz Ali


// Program: CS213 - 20220325 - 20220198 - 20221091
// Purpose: Demonstrate use of bmplip for handling
//          bmp colored and grayscale images
//          Program load a gray image and store in another file
// Author:  Marwan Sherif Abdellatif - AbdelAziz Ali Alarabi - Deyaa Ahmed Saber
// Date:    9 Oct 2023
// Version: 1.0

typedef long long ll;
using namespace std;
unsigned char image[SIZE][SIZE];
unsigned char image2[SIZE][SIZE];
unsigned char image3[SIZE][SIZE];
unsigned char temp[SIZE][SIZE];

void sth() {
    for (int i{0};i<SIZE;i++){
        for(int j{0};j<SIZE;j++){
            temp[i][j]=255;
        }
    }
}
vector<int> v(4);
set<int> s;
bool wrong_input = false;

void loadImage();

void loadImage2();

void saveImage();

void saveImage_3();

// Black and White filter
void filter_1() {
    for (auto &i: image) {
        for (unsigned char &j: i) {
//            Check whether the pixel is above 111, if it's then make it white, otherwise make it black.
// We used "111" to get a better version.
            if (j > 111) {
                j = 255;
            } else {
                j = 0;
            }
        }
    }
}

// Invert filter
void filter_2() {
//    We invert the image by subtracting each pixel's value from 255.
    for (auto &i: image) {
        for (unsigned char &j: i) {
            j = 255 - j;
        }
    }
}

// Merge filter
void filter_3() {
//   Each pixel of the new image will equal to the average of the corresponding 2 pixels to avoid out-of-range errors.
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            image[i][j] = (image2[i][j] + image[i][j]) / 2;
        }
    }
}

// Flip filter
void filter_4(int num) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
//                                      Vertically               Horizontally
            image3[i][j] = (num == 2) ? image[i][SIZE - 1 - j] : image[SIZE - 1 - i][j];
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}
// Rotate Image
void filter_5(int num) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
//                                      90-deg                                180-deg
            image3[i][j] = (num == 1) ? image[SIZE - 1 - j][i] : (num == 2) ? image[SIZE - 1 - i][SIZE - j - 1]
                                                                 //                                270-deg
                                                                            : image[j][SIZE - i - 1];
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}

// Lightness or Darkness filter
/*
 Here are the followed steps of this filter:
 Lightness:
 The more pixel value closer to 255 the mor it will be lighter.
 So we first get the invert of the pixel and divide it by 2 (to handle the worst case which is when j = 0, then it will be 127).
 Darkness:
 We here try to make the value of each pixel closer to 0 (black) by dividing it by 2 (factor of darkness).
 */
void filter_6(int num) {
    for (auto &i: image) {
        for (unsigned char &j: i) {
//                       lightness          darkness
            (num == 1) ? j += (255 - j) / 2 : j /= 2;
        }
    }
}

// Detect image edges filter
void filter_7() {
    ll avg = 0;
//    First, we store a draft white image @ the "image3" object to use it later to draw the edges.
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            avg += image[i][j];
            image3[i][j] = 255;
        }
    }
    avg /= (SIZE * SIZE);
//    The edges detecting process goes as follows:
/*
 We check for each pixel if its value is greater than the average one and the next-to-the-right or the up pixel is less than the average or the pixel is less than the average and the next-to-the-right or the up pixel is greater than the average.
 If this holds, then it means that there's a difference between the 2 colors, then we have to mark this as an edge.
 */
    for (int i = 0; i < SIZE - 1; ++i) {
        for (int j = 0; j < SIZE - 1; ++j) {
            (avg < image[i][j] && (image[i][j + 1] < avg || image[i + 1][j] < avg) ||
             (avg > image[i][j] && (image[i][j + 1] > avg || image[i + 1][j] > avg))) ? image3[i][j] = 0, "Marwan"
                                                                                      : "01156576599";
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}

// Enlarge the image filter.
void filter_8(int num) {

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (num == 1) {
                image3[i][j] = image[i / 2][j / 2];
            } else if (num == 2) {
                image3[i][j] = image[i / 2][j / 2 + SIZE / 2];
            } else if (num == 3) {
                image3[i][j] = image[i / 2 + SIZE / 2][j / 2];
            } else {
                image3[i][j] = image[i / 2 + SIZE / 2][j / 2 + SIZE / 2];
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}

// Shrink Image
void filter_9(int num) {
    // 2 for 1/2 , 3 for 1/3 , 4 for 1/4
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i < SIZE / num && j < SIZE / num) {
                image3[i][j] = image[i * num][j * num];
            } else {
                image3[i][j] = 255;
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}

// Mirror Image
void filter_10(int num) {
    int index_2 = 0;
    if (num == 1) {
        for (auto &i: image) {
            index_2 = 1;
            for (int j = SIZE / 2; j < SIZE; j++) {
                i[j] = i[SIZE / 2 - index_2];
                index_2++;
            }
        }
    }
    if (num == 2) {
        for (auto &i: image) {
            index_2 = 0;
            for (int j = SIZE / 2 - 1; j >= 0; j--) {
                i[j] = i[SIZE / 2 + index_2];
                index_2++;
            }
        }
    }
    if (num == 3) {
        for (int i = SIZE / 2; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                image[i][j] = image[SIZE / 2 - 1 - index_2][j];
            }
            index_2++;
        }
    }
    if (num == 4) {
        for (int i = SIZE / 2 - 1; i >= 0; i--) {
            for (int j = 0; j < SIZE; j++) {
                image[i][j] = image[SIZE / 2 + index_2][j];
            }
            index_2++;
        }
    }

}

// Shuffle Image
void filter_11(int sum) {
    if (sum != 10 || s.size() != 4 || wrong_input) return void(cout << "Wrong input\n");
    int index_1, index_2, index_3, index_4;
    for (int i = 0; i < 4; ++i) {
        if ((i + 1) & 1) index_2 = 0;
        else index_2 = 127;
        if (i <= 1) index_1 = 0;
        else index_1 = 127;
        if (v[i] & 1) index_4 = 0;
        else index_4 = 127;
        if (v[i] <= 2) index_3 = 0;
        else index_3 = 127;
        for (int j = 0; j < 127; ++j) {
            for (int k = 0; k < 127; ++k) {
                image3[j + index_1][k + index_2] = image[j + index_3][k + index_4];
            }
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            image[i][j] = image3[i][j];
        }
    }
}

// Blur Image
void filter_12() {
    for (int k = 0; k < 5; ++k) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                int sum = 0;
                int cnt = 0;

                // Iterate through the neighboring pixels
                for (int x = i - 2; x <= i + 2; x++) {
                    for (int y = j - 2; y <= j + 2; y++) {
                        // Check if indices are within the valid range
                        if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
                            sum += image[x][y];  // Add the pixel value to the sum
                            cnt++;  // Increment the count of valid pixels
                        }
                    }
                }

                // Calculate the average and assign it to the blurred image
                image[i][j] = (sum / cnt);
            }
        }
    }
}

// Crop Image
void filter_13(int x, int y, int l, int w) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (!(i >= x && i <= x + l && j >= y && j <= y + w)) image[i][j] = 255;
        }
    }
}

// Skew Horizontally / Vertically
void filter_14(char type,double deg){
    double left = tan(deg*3.14159/180.0)*SIZE; // The part that we will leave
    double step_for_original=(double) SIZE/(SIZE-left);
    double step_for_new= (double) left/SIZE;
    double here=0; // for the new pic's pointer to add its step to it
    double curr=0; // for the old pic's pointer to add its step to it

    if(type=='H'){
        // nmshy row row
        for(int i{0};i<SIZE;i++){// rows are the same either way
            curr=0; // start from the first row either way (curr+= step_for_original) to move to another column
            for(int j=left-here;j<(SIZE-here);j++){// the columns get shifted to the left after each new row and the edges change to the left to by a space called "step_for_new" to be at "here"
                // let's get the average of pixels between ( old_curr ---> new_curr) and put it in the new pixel in a temp
                int old_curr=max(0,(int)ceil(curr-step_for_original)); // and index must be an non-negative integer, so ceil;
                int new_curr=min(SIZE,(int)ceil(curr+step_for_original)); // and obviously not more than the SIZE
                        //let's average!!
                int sum=0;
                int pixels= new_curr-old_curr;

                for(int c{old_curr};c<new_curr;c++){
                      sum+=image[i][c];
                      pixels++;
                  }
                  temp[i][j]=sum/max(1,pixels);
                  curr+=step_for_original;
            }
            here+=step_for_new;
        }
            }
    else {
        // nmshy column column mn t7t le foo2
        for(int j{0};j<SIZE;j++){// columns are the same either way for every lap
            curr=0; // start from the first row either way (curr+= step_for_original) to move to another row
            for(int i=left-here;i<(SIZE-here);i++){// the rows get shifted up after each new column and the edges change to up to by a space called "step_for_new" to be at "here"
                // let's get the average of pixels between ( old_curr ---> new_curr) and put it in the new pixel in a temp
                int old_curr=max(0,(int)ceil(curr-step_for_original)); // and index must be an non-negative integer, so ceil;
                int new_curr=min(SIZE,(int)ceil(curr+step_for_original)); // and obviously not more than the SIZE
                //let's average!!
                int sum=0;
                int pixels= new_curr-old_curr;

                for(int c{old_curr};c<new_curr;c++){
                    sum+=image[c][j];
                    pixels++;
                }
                temp[i][j]=sum/max(1,pixels);
                curr+=step_for_original;


            }
            here+=step_for_new;
        }

    }
    for(int i{0};i<SIZE;i++){
        for(int j{0};j<SIZE;j++){
            image[i][j]=temp[i][j];
        }
    }

}

int main() {
    loadImage();
    sth();
    while (true){
        cout << "\nPlease Enter what do you want to do to edit image\n"
                "1- Black & White Filter\n"
                "2- Invert Filter\n"
                "3- Merge Filter\n"
                "4- Flip Image\n"
                "5- Rotate Image\n"
                "6- Darken and Lighten Image\n"
                "7- Detect Image Edges \n"
                "8- Enlarge Image\n"
                "9- Shrink Image\n"
                "10- Mirror Image\n"
                "11- Shuffle Image\n"
                "12- Blur Image\n"
                "13- Crop Image\n"
                "14- Skew image\n"
                "0 - Exit\n"
                "#> ";
        int num;
        cin >> num;
        if(num==0){
            break;
        }
        if (num == 1) {
            filter_1();
        }

        if (num == 2) {
            filter_2();
        }

        if (num == 3) {
            loadImage2();
            filter_3();
        }

        if (num == 4) {
            cout << "Please Enter:"
                    "\n1) for horizontal"
                    "\n2) for vertical"
                    "\n"
                    "#> ";
            int x;
            cin >> x;
            filter_4(x);
        }

        if (num == 5) {
            cout << "Please Enter:"
                    "\n1) for 90-deg"
                    "\n2) for 180-deg"
                    "\n3) for 270-deg"
                    "\n"
                    "#> ";
            int x;
            cin >> x;
            filter_5(x);
        }

        if (num == 6) {
            cout << "Please Enter:"
                    "\n1) for lighten by 50%"
                    "\n2) for darken by 50%"
                    "\n"
                    "#> ";
            int x;
            cin >> x;
            filter_6(x);
        }

        if (num == 7) {
            filter_7();
        }

        if (num == 8) {
            cout << "Please Enter:"
                    "\n1) for 1st quarter"
                    "\n2) for 2ec quarter"
                    "\n3) for 3rd quarter"
                    "\n4) for 4th quarter"
                    "\n"
                    "#> ";
            int x;
            cin >> x;
            filter_8(x);
        }

        if (num == 9) {
            cout
                    << "Please Enter:"
                       "\n2) for 1/2 original dimensions"
                       "\n3) for 1/3 original dimensions"
                       "\n4) for 1/4 original dimensions"
                       "\n"
                       "#> ";
            int x;
            cin >> x;
            filter_9(x);
        }

        if (num == 10) {
            cout << "Please Enter:"
                    "\n1) for Left 1/2"
                    "\n2) for Right 1/2"
                    "\n3) for Upper 1/2"
                    "\n4) for Lower 1/2"
                    "\n"
                    "#> ";
            int x;
            cin >> x;
            filter_10(x);
        }

        if (num == 11) {
            cout << "Enter 4 unique space-separated numbers from 1 up-to 4 in any order.\n";
            int sum = 0;
            for (int i = 0; i < 4; ++i) {
                int x;
                cin >> x;
                if (x < 0) {
                    wrong_input = true;
                }
                sum += x;
                v[i] = x;
                s.insert(x);
            }
            filter_11(sum);
        }

        if (num == 12) {
            filter_12();
        }

        if (num == 13) {
            int x, y, l, w;
            cout << "Please Enter x, y, l, w\n";
            cin >> x >> y >> l >> w;
            filter_13(x, y, l, w);
        }
        if(num==14){
            cout<<" skew (H)orizontally or (V)ertically ? \n";
            char type; cin>>type;
            if(type=='v'|| type=='V'){type='V';}
            else if(type=='h' ||type=='H'){type='H';}
            else{
                while(type!='v' && type!='V'&& type!='H'&& type!='h'){
                    cout<<"Invalid character !!\n";
                    cout<<" skew (H)orizontally or (V)ertically ? \n";
                }
            }
            cout<<" OKAY!! Please enter the degree (0-->45) : ";
            double degree; cin>>degree;
            while(degree<0 || degree>45){
                cout<<"Invalid degree!\n";
                cout<<"please enter a degree between (0-->45): ";
                cin>>degree;
            }
            filter_14(type,degree);
        }
        saveImage();

    }
    return 0;
}

//_________________________________________
void loadImage() {
    char imageFileName[100];

    // Get gray scale image file name
    cout << "Enter the source image file name: ";
    cin >> imageFileName;
    // Add to it .bmp extension and load image
    strcat(imageFileName, ".bmp");
    readGSBMP(imageFileName, image);
}

//_________________________________________
void loadImage2() {
    char imageFileName[100];

    // Get gray scale image file name
    cout << "Enter the source image file name: ";
    cin >> imageFileName;

    // Add to it .bmp extension and load image
    strcat(imageFileName, ".bmp");
    readGSBMP(imageFileName, image2);
}

//_________________________________________
void saveImage() {
    char imageFileName[100];

    // Get gray scale image target file name
    cout << "Enter the target image file name: ";
    cin >> imageFileName;
    // Add to it .bmp extension and load image
    strcat(imageFileName, ".bmp");
    writeGSBMP(imageFileName, image);
}



