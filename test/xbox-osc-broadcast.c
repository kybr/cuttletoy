#include <fcntl.h> // open
#include <stdio.h> // perror
#include <linux/input.h> // struct input_event
#include <unistd.h> // read
#include <stdlib.h> // exit

#include <lo/lo.h>

void error(int num, const char *m, const char *path) {
  printf("FAIL FAIL\n");
  exit(1);
}

int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data) {
  int i;

  lo_address* p = (lo_address*)user_data;
  int result = lo_send_message(*p, path, data); // repeat message
  if (result == -1) {
    printf("OSC error %d: %s\n", lo_address_errno(*p), lo_address_errstr(*p));
    exit(1);
  }
  
  printf("generic handler; path: <%s>\n", path);
  for (i = 0; i < argc; i++) {
    printf("arg %d '%c' ", i, types[i]);
    lo_arg_pp((lo_type)types[i], argv[i]);
    printf("\n");
  }
  printf("\n");
  fflush(stdout);
  
  return 1;
}



int main() {
  int fd;
  if ((fd = open("/dev/input/event0", O_RDONLY)) < 0) {
    perror("Cannot find device");
    return 1;
  }

  lo_address t = lo_address_new("255.255.255.255", "7770");

  lo_server_thread server = lo_server_thread_new("10001", error);
  lo_server_thread_add_method(server, NULL, NULL, generic_handler, &t);
  lo_server_thread_start(server);

  struct input_event event;

  while (1) {
    if (read(fd, &event, sizeof(event)) != sizeof(event)) {
      perror("Fail to read complete event");
      break;
    }
    
    //printf("type:%d code:%d value:%d\n", event.type, event.code, event.value);

    if (event.type == EV_KEY) {
      int result = 0;
      switch (event.code) {
        case 304: result = lo_send(t, "/button/south", "i", event.value); break;
        case 305: result = lo_send(t, "/button/east", "i", event.value); break;
        case 307: result = lo_send(t, "/button/north", "i", event.value); break;
        case 308: result = lo_send(t, "/button/west", "i", event.value); break;
        case 310: result = lo_send(t, "/button/tl", "i", event.value); break;
        case 311: result = lo_send(t, "/button/tr", "i", event.value); break;
        case 317: result = lo_send(t, "/button/thumbl", "i", event.value); break;
        case 318: result = lo_send(t, "/button/thumbr", "i", event.value); break;
        case 314: result = lo_send(t, "/button/select", "i", event.value); break;
        case 315: result = lo_send(t, "/button/start", "i", event.value); break;
        case 316: result = lo_send(t, "/button/mode", "i", event.value); break;
        default:
	  printf("Bad event code\n");
	  exit(1);
          break;
      }
      if (result == -1) {
        printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
	exit(1);
      }
    }


    if (event.type == EV_ABS) {
      int result = 0;
      switch (event.code) {
        // Fuzz      16
        // Flat     128
        // Min   -32768
        // Max    32767
	case 0: result = lo_send(t, "/stick/x", "f", event.value / 32768.0f); break;
        case 1: result = lo_send(t, "/stick/y", "f", event.value / 32768.0f); break;
        case 3: result = lo_send(t, "/stick/rx", "f", event.value / 32768.0f); break;
        case 4: result = lo_send(t, "/stick/ry", "f", event.value / 32768.0f); break;

	
	case 2: result = lo_send(t, "/trigger/z", "f", event.value / 1023.0f); break;
	case 5: result = lo_send(t, "/trigger/rz", "f", event.value / 1023.0f); break;

        // Value      0
        // Min       -1
        // Max        1
	case 16: result = lo_send(t, "/hat/x", "i", event.value); break;
	case 17: result = lo_send(t, "/hat/y", "i", event.value); break;

	default:
	  break;
      }
      if (result == -1) {
        printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
	exit(1);
      }
    }
  }
}
/*
Input driver version is 1.0.1
Input device ID: bus 0x3 vendor 0x45e product 0x2ea version 0x301
Input device name: "Microsoft X-Box One S pad"
Supported events:
  Event type 0 (EV_SYN)
  Event type 1 (EV_KEY)
    Event code 304 (BTN_SOUTH)
    Event code 305 (BTN_EAST)
    Event code 307 (BTN_NORTH)
    Event code 308 (BTN_WEST)
    Event code 310 (BTN_TL)
    Event code 311 (BTN_TR)
    Event code 314 (BTN_SELECT)
    Event code 315 (BTN_START)
    Event code 316 (BTN_MODE)
    Event code 317 (BTN_THUMBL)
    Event code 318 (BTN_THUMBR)
  Event type 3 (EV_ABS)
    Event code 0 (ABS_X)
      Value    932
      Min   -32768
      Max    32767
      Fuzz      16
      Flat     128
    Event code 1 (ABS_Y)
      Value   1977
      Min   -32768
      Max    32767
      Fuzz      16
      Flat     128
    Event code 2 (ABS_Z)
      Value      0
      Min        0
      Max     1023
    Event code 3 (ABS_RX)
      Value    772
      Min   -32768
      Max    32767
      Fuzz      16
      Flat     128
    Event code 4 (ABS_RY)
      Value   -351
      Min   -32768
      Max    32767
      Fuzz      16
      Flat     128
    Event code 5 (ABS_RZ)
      Value      0
      Min        0
      Max     1023
    Event code 16 (ABS_HAT0X)
      Value      0
      Min       -1
      Max        1
    Event code 17 (ABS_HAT0Y)
      Value      0
      Min       -1
      Max        1
  Event type 21 (EV_FF)
    Event code 80 (FF_RUMBLE)
    Event code 81 (FF_PERIODIC)
    Event code 88 (FF_SQUARE)
    Event code 89 (FF_TRIANGLE)
    Event code 90 (FF_SINE)
    Event code 96 (FF_GAIN)
*/
