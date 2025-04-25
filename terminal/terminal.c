#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

//const char* glyph[512] = {"\u00a4","\u263a","\u263b","\u00a6","\u2666","\u2663","\u2660","\u2022","\u00a8","\u00a9","\u00ae","\u2642","\u2640","\u00af","\u00b3","\u263c","\u00b4","\u00b8","\u2195","\u203c","\u00b6","\u00a7","\u00b9","\u21a8","\u2191","\u2193","\u2192","\u2190","\u221f","\u2194","\u00be","\u00c0","\u0020","\u0021","\u0022","\u0023","\u0024","\u0025","\u0026","\u0027","\u0028","\u0029","\u002a","\u002b","\u002c","\u002d","\u002e","\u002f","\u0030","\u0031","\u0032","\u0033","\u0034","\u0035","\u0036","\u0037","\u0038","\u0039","\u003a","\u003b","\u003c","\u003d","\u003e","\u003f","\u0040","\u0041","\u0042","\u0043","\u0044","\u0045","\u0046","\u0047","\u0048","\u0049","\u004a","\u004b","\u004c","\u004d","\u004e","\u004f","\u0050","\u0051","\u0052","\u0053","\u0054","\u0055","\u0056","\u0057","\u0058","\u0059","\u005a","\u005b","\u005c","\u005d","\u005e","\u005f","\u0060","\u0061","\u0062","\u0063","\u0064","\u0065","\u0066","\u0067","\u0068","\u0069","\u006a","\u006b","\u006c","\u006d","\u006e","\u006f","\u0070","\u0071","\u0072","\u0073","\u0074","\u0075","\u0076","\u0077","\u0078","\u0079","\u007a","\u007b","\u007c","\u007d","\u007e","\u00c1","\u00c7","\u00fc","\u00e9","\u00e2","\u00e4","\u00e0","\u00e5","\u00e7","\u00ea","\u00eb","\u00e8","\u00ef","\u00ee","\u00ec","\u00c4","\u00c5","\u00c9","\u00e6","\u00c6","\u00f4","\u00f6","\u00f2","\u00fb","\u00f9","\u00ff","\u00d6","\u00dc","\u00a2","\u00a3","\u00a5","\u20a7","\u0192","\u00e1","\u00ed","\u00f3","\u00fa","\u00f1","\u00d1","\u00aa","\u00ba","\u00bf","\u2310","\u00ac","\u00bd","\u00bc","\u00a1","\u00ab","\u00bb","\u2591","\u2592","\u00c2","\u2502","\u2524","\u00c3","\u00c8","\u00ca","\u00cb","\u00cc","\u00cd","\u00ce","\u00cf","\u00d0","\u00d2","\u2510","\u2514","\u2534","\u252c","\u251c","\u2500","\u253c","\u00d3","\u00d4","\u00d5","\u00d7","\u00d8","\u00d9","\u00da","\u00db","\u00dd","\u00de","\u00e3","\u00f0","\u00f5","\u00f8","\u00fd","\u00fe","\u011e","\u011f","\u0130","\u2518","\u250c","\u2588","\u0131","\u0152","\u0153","\u015e","\u015f","\u00df","\u0393","\u03c0","\u0160","\u0161","\u00b5","\u0178","\u03a6","\u017d","\u03a9","\u017e","\u221e","\u02c6","\u02dc","\u2229","\u2261","\u00b1","\u2265","\u2264","\u2320","\u2321","\u00f7","\u2248","\u00b0","\u2219","\u00b7","\u221a","\u207f","\u00b2","\u25a0","\u2014","\u2018","\u2019","\u201a","\u201c","\u201d","\u201e","\u2020","\u2021","\u2026","\u2030","\u2039","\u203a","\u20ac","\u2122","\u0102","\u0103","\u0104","\u0105","\u0106","\u0107","\u010c","\u010d","\u010e","\u010f","\u0111","\u0118","\u0119","\u011a","\u011b","\u0139","\u013a","\u013d","\u013e","\u0141","\u0142","\u0143","\u0144","\u0147","\u0148","\u0150","\u0151","\u0154","\u0155","\u0158","\u0159","\u015a","\u015b","\u0162","\u0163","\u0164","\u0165","\u016e","\u016f","\u0170","\u0171","\u0179","\u017a","\u017b","\u017c","\u0218","\u0219","\u021a","\u021b","\u02c7","\u02d8","\u02d9","\u02db","\u02dd","\u0108","\u0109","\u010a","\u010b","\u011c","\u011d","\u0120","\u0121","\u0124","\u0125","\u0126","\u0127","\u0134","\u0135","\u015c","\u015d","\u016c","\u016d","\u0174","\u0175","\u0176","\u0177","\u1e40","\u1e41","\u0100","\u0101","\u0112","\u0113","\u0116","\u0117","\u0122","\u0123","\u012a","\u012b","\u012e","\u012f","\u0136","\u0137","\u013b","\u013c","\u0145","\u0146","\u014c","\u014d","\u0156","\u0157","\u016a","\u016b","\u0172","\u0173","\u0400","\u0402","\u0403","\u0404","\u0409","\u040a","\u040b","\u040c","\u040d","\u040e","\u040f","\u0411","\u0414","\u0416","\u0417","\u0418","\u0419","\u041b","\u03a0","\u0423","\u0426","\u0427","\u0428","\u0429","\u042a","\u042b","\u042c","\u042d","\u042e","\u042f","\u0431","\u0432","\u0433","\u0434","\u0436","\u0437","\u0438","\u0439","\u043a","\u043b","\u043c","\u043d","\u043f","\u0442","\u0444","\u0446","\u0447","\u0448","\u0449","\u044a","\u044b","\u044c","\u044d","\u044e","\u044f","\u0450","\u0452","\u0453","\u0454","\u0459","\u045a","\u045b","\u045c","\u045d","\u045e","\u045f","\u0490","\u0491","\u2116","\u0492","\u0493","\u0494","\u0495","\u0496","\u0497","\u0498","\u0499","\u049a","\u049b","\u049c","\u049d","\u04a0","\u04a1","\u04a2","\u04a3","\u04a4","\u04a5","\u04aa","\u04ab","\u04af","\u04b2","\u04b3","\u04b6","\u04b7","\u04b8","\u04b9","\u04ba","\u04bb","\u04d2","\u04d3","\u04d6","\u04d7","\u04d8","\u04d9","\u04e2","\u04e3","\u04e6","\u04e7","\u04e8","\u04e9","\u04ee","\u04ef","\u04f0","\u04f1","\u04f2","\u04f3","\u04f8","\u04f9","\u20ae","\u2260","\u2208","\u2205","\u2016","\u2017","\u2032","\u2033","\u21b5","\u21d0","\u21d1","\u21d2","\u21d3","\u21d4","\u21d5","\u2206","\u2227","\u2228","\u222a","\u2302"};

void handle() { endwin(); }

int main(void) {
  signal(SIGTERM, handle);

  //setlocale(LC_ALL, "");
  // printf("%s\n", setlocale(LC_ALL, NULL));
  // exit(1);
  initscr();
  cbreak();
  noecho();
  clear();
  curs_set(0);

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);

  init_pair(8, COLOR_BLACK, COLOR_RED);
  init_pair(9, COLOR_BLACK, COLOR_GREEN);
  init_pair(10, COLOR_BLACK, COLOR_YELLOW);
  init_pair(11, COLOR_BLACK, COLOR_BLUE);
  init_pair(12, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(13, COLOR_BLACK, COLOR_CYAN);
  init_pair(14, COLOR_BLACK, COLOR_WHITE);

  init_pair(15, COLOR_WHITE, COLOR_WHITE);
  init_pair(16, COLOR_RED, COLOR_WHITE);
  init_pair(17, COLOR_GREEN, COLOR_WHITE);
  init_pair(18, COLOR_YELLOW, COLOR_WHITE);
  init_pair(19, COLOR_BLUE, COLOR_WHITE);
  init_pair(20, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(21, COLOR_CYAN, COLOR_WHITE);

  // bkgd(COLOR_PAIR(15));

  // move(x, y);
  // printw("...");

  while (1) {
    //clear();
    int yi = random() % LINES;
    int xi = random() % COLS;
    unsigned char c = random() & 255;
    if (c < 32 || c == 127 || c == 255) {
      continue;
    }
    attron(COLOR_PAIR(1 + random() % 7));
    mvaddch(yi, xi, c);
    refresh();
    usleep(10000);
  }
}
