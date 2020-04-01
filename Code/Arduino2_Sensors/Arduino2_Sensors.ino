// Function: handles getting data from 6 distance sensors
//           and sending the data to the drivetrain Arduino (master)

/*
Collision Prevention System - Arduino 2 (Sensors)

Outline:
1. Receive input sensors
2. Average out data over 3-5 cycles to account for outliers
3. Send data to Ar4 (LCD)
*/

#include <Wire.h>
#include <NewPing.h>

#define MAX_DISTANCE 200
#define FRONT_THRESHOLD 40
#define BACK_THRESHOLD 40
#define SIDE_THRESHOLD 15
int num_sensors = 6;

int fl, fm, fr, bl, bm, br;
double avg_fl = 0.0;
double avg_fm = 0.0;
double avg_fr = 0.0;
double avg_bl = 0.0;
double avg_bm = 0.0;
double avg_br = 0.0;
int averages[] = {0,0,0,0,0,0};

// use the following as circular array
int fl_arr[] = {0,0,0,0,0}; // adjust after algorithm created
int fm_arr[] = {0,0,0,0,0};
int fr_arr[] = {0,0,0,0,0};
int bl_arr[] = {0,0,0,0,0};
int bm_arr[] = {0,0,0,0,0};
int br_arr[] = {0,0,0,0,0};
int beginning = 1;
int arr_end = 1;
int count = 1; // keeps track of which reading is being taken
int i;

NewPing sensors[6] = {
  // NewPing(Trigger, Echo, MaxDistance)
  NewPing(13,12,MAX_DISTANCE), // fl, index 0, sensor 1
  NewPing(11,10,MAX_DISTANCE), // fm, index 1, sensor 2
  NewPing(9,8,MAX_DISTANCE),   // fr, index 2, sensor 3
  NewPing(3,2,MAX_DISTANCE),   // br, index 3, sensor 4
  NewPing(5,4,MAX_DISTANCE),   // bm, index 4, sensor 5
  NewPing(7,6,MAX_DISTANCE),   // bl, index 5, sensor 6
};

void setup(){
  Wire.begin(2);
}

void loop(){
  for(i = 0; i < 6; i++){
    if(i == 0){ // sensor 1
      fl_arr[arr_end] = sensors[0].ping_cm();
    }
    else if(i == 1){ // sensor 2
      fm_arr[arr_end] = sensors[1].ping_cm();
    }
    else if(i == 2){ // sensor 3
      fr_arr[arr_end] = sensors[2].ping_cm();
    }
    else if(i == 3){ // sensor 4
      br_arr[arr_end] = sensors[3].ping_cm();
    }
    else if(i == 4){ // sensor 5
      bm_arr[arr_end] = sensors[4].ping_cm();
    }
    else{ // sensor 6
      bl_arr[arr_end] = sensors[5].ping_cm();
    }
  }

  if(count % 3 == 0){ // 3rd reading
    // get updated averages
    if(arr_end > beginning){ // no loop around
      avg_fl = (fl_arr[beginning] + fl_arr[beginning+1] + fl_arr[arr_end]) / 3.0;
      avg_fm = (fm_arr[beginning] + fm_arr[beginning+1] + fm_arr[arr_end]) / 3.0;
      avg_fr = (fr_arr[beginning] + fr_arr[beginning+1] + fr_arr[arr_end]) / 3.0;
      avg_bl = (bl_arr[beginning] + bl_arr[beginning+1] + bl_arr[arr_end]) / 3.0;
      avg_bm = (bm_arr[beginning] + bm_arr[beginning+1] + bm_arr[arr_end]) / 3.0;
      avg_br = (br_arr[beginning] + br_arr[beginning+1] + br_arr[arr_end]) / 3.0;
    }
    else{ // end < beginning
      if(arr_end > 0){ // end is at least at index 1
        avg_fl = (fl_arr[beginning] + fl_arr[arr_end-1] + fl_arr[arr_end]) / 3.0;
        avg_fm = (fm_arr[beginning] + fm_arr[arr_end-1] + fm_arr[arr_end]) / 3.0;
        avg_fr = (fr_arr[beginning] + fr_arr[arr_end-1] + fr_arr[arr_end]) / 3.0;
        avg_bl = (bl_arr[beginning] + bl_arr[arr_end-1] + bl_arr[arr_end]) / 3.0;
        avg_bm = (bm_arr[beginning] + bm_arr[arr_end-1] + bm_arr[arr_end]) / 3.0;
        avg_br = (br_arr[beginning] + br_arr[arr_end-1] + br_arr[arr_end]) / 3.0;
      }
      else{ // end is at index 0
        avg_fl = (fl_arr[beginning] + fl_arr[beginning+1] + fl_arr[arr_end]) / 3.0;
        avg_fm = (fm_arr[beginning] + fm_arr[beginning+1] + fm_arr[arr_end]) / 3.0;
        avg_fr = (fr_arr[beginning] + fr_arr[beginning+1] + fr_arr[arr_end]) / 3.0;
        avg_bl = (bl_arr[beginning] + bl_arr[beginning+1] + bl_arr[arr_end]) / 3.0;
        avg_bm = (bm_arr[beginning] + bm_arr[beginning+1] + bm_arr[arr_end]) / 3.0;
        avg_br = (br_arr[beginning] + br_arr[beginning+1] + br_arr[arr_end]) / 3.0;
      }
    }

    // Send averages to Ar3
    // cast averages to be chars
//    averages = {int(avg_fl)+'0', int(avg_fm)+'0', int(avg_fr)+'0', int(avg_br)+'0', int(avg_bm)+'0', int(avg_bl)+'0'};
    Wire.beginTransmission(3);
//    Wire.write('S');
//    Wire.write(averages);
    Wire.write(int(avg_fl)+'0');
    Wire.write(int(avg_fm)+'0');
    Wire.write(int(avg_fr)+'0');
    Wire.write(int(avg_br)+'0');
    Wire.write(int(avg_bm)+'0');
    Wire.write(int(avg_bl)+'0');
    Wire.endTransmission();
    Wire.endTransmission();

    // Send averages to Ar4
//    Wire.beginTransmission(4);
//    Wire.write(averages);
//    Wire.endTransmission();

    // move beginning to start the next interval
    if(arr_end < 4){ // not at the last index
      beginning = arr_end + 1;
    }
    else{
      beginning = 0;
    }

  }
  else{ // just received 1st or 2nd reading in the cycle
    count++;
    if(arr_end != 4){ // not at the last index
      arr_end++;
    }
    else{
      arr_end = 0;
    }
    delay(50);
  }
  
}
