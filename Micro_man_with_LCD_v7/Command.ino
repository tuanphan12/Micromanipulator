//must double check if the magnet is North or South
int x_home(int hall_reading_) {
  if (hall_reading_ < 1000){
    return 1;
  }
  else{
    return 0;
  }
}

void switching_motor(float motorAxis_) {
  int state_ = (int) motorAxis_;
  switch (state_){
    case 0: //axis 1 (mounting platform)
      axis = stp1;
      axisStr = "Mounting Platform";   
      break;
    case 1: //axis 2 (bottom platform)
      axis = stp2;
      axisStr = "Bottom Platform";
      break;   
    case 2: //axis 2 (bottom platform)
      axis = stp3;
      axisStr = "Z-Axis";
      break;    
  }
}

