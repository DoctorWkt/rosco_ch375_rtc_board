// Code to test the DS1302 device
// (c) 2025 Warren Toomey GPL3

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ds1302.h"

// These are in the asmcode.asm file
extern void rtcwrite(uint16_t value);
extern uint8_t rtcread(uint8_t cmd);

// See Table 3: Register Address/Definition
// from the DS1302 datasheet to explain the
// code in the following functions. Essentially,
// data is BCD encoded in two sections: the tens
// column and the ones column.

// Get the number of seconds
int get_seconds(void) {
  uint8_t val = rtcread(RTC_RDSECONDS);
  int secs = 10 * ((val >> 4) & 0x7) + (val & 0xf);
  return (secs);
}

// Get the number of minutes
int get_minutes(void) {
  uint8_t val = rtcread(RTC_RDMINUTES);
  int mins = 10 * ((val >> 4) & 0x7) + (val & 0xf);
  return (mins);
}

// Get the number of hours
int get_hours(void) {
  uint8_t val = rtcread(RTC_RDHOURS);
  int hours = 10 * ((val >> 4) & 0x3) + (val & 0xf);
  return (hours);
}

// Get the number of days
int get_days(void) {
  uint8_t val = rtcread(RTC_RDDAYS);
  int days = 10 * ((val >> 4) & 0x3) + (val & 0xf);
  return (days);
}

// Get the number of months
int get_months(void) {
  uint8_t val = rtcread(RTC_RDMONTHS);
  int months = 10 * ((val >> 4) & 0x1) + (val & 0xf);
  return (months);
}

// Get the number of years
int get_years(void) {
  uint8_t val = rtcread(RTC_RDYEARS);
  int years = 10 * ((val >> 4) & 0xf) + (val & 0xf);
  return (years);
}

// Disable the write protect on the chip
void disable_wp(void) {
  uint16_t cmd_data = RTC_WRPROT;	// Data is 0x00
  rtcwrite(cmd_data);
}

// Set the number of seconds
void set_seconds(int secs) {
  uint16_t cmd_data = ((secs / 10) << 12) + ((secs % 10) << 8);
  cmd_data |= RTC_SECONDS;
  rtcwrite(cmd_data);
}

// Set the number of minutes
void set_minutes(int mins) {
  uint16_t cmd_data = ((mins / 10) << 12) + ((mins % 10) << 8);
  cmd_data |= RTC_MINUTES;
  rtcwrite(cmd_data);
}

// Set the number of hours
void set_hours(int hours) {
  uint16_t cmd_data = ((hours / 10) << 12) + ((hours % 10) << 8);
  cmd_data |= RTC_HOURS;
  rtcwrite(cmd_data);
}

// Set the number of days
void set_days(int days) {
  uint16_t cmd_data = ((days / 10) << 12) + ((days % 10) << 8);
  cmd_data |= RTC_DAYS;
  rtcwrite(cmd_data);
}

// Set the number of months
void set_months(int months) {
  uint16_t cmd_data = ((months / 10) << 12) + ((months % 10) << 8);
  cmd_data |= RTC_MONTHS;
  rtcwrite(cmd_data);
}

// Set the number of years
void set_years(int years) {
  years = years % 100;		// Keep only the lower two digits
  uint16_t cmd_data = ((years / 10) << 12) + ((years % 10) << 8);
  cmd_data |= RTC_YEARS;
  rtcwrite(cmd_data);
}

// Print the current date/time
void print_date_time(void) {
  printf("DS1302 current date/time: %02d/%02d/%02d %02d:%02d:%02d\n",
	 get_years(), get_months(), get_days(),
	 get_hours(), get_minutes(), get_seconds());
}

void kmain() {
  char buf[100];
  int hour, min, sec;
  int year, mon, day;

  // Print out the date/time from the chip
  while (1) {
    print_date_time();
    printf("Set the time (y/n)? ");

    buf[0] = 0;
    if (fgets(buf, 99, stdin) && (buf[0] == 'Y' || buf[0] == 'y')) {
      printf("Enter using the format YYYY/MM/DD HH:MM:SS: ");

      if (fgets(buf, 99, stdin)) {
	// Check the format
	if (buf[4] != '/' || buf[7] != '/' || buf[10] != ' ' ||
	    buf[13] != ':' || buf[16] != ':') {
	  printf("Input was not YYYY/MM/DD HH:MM:SS format\n");
	  exit(0);
	}

	// Get the data
	year = atoi(&buf[0]); mon = atoi(&buf[5]); day = atoi(&buf[8]);
	hour = atoi(&buf[11]); min = atoi(&buf[14]); sec = atoi(&buf[17]);

	// Enable writing and write the new data
	disable_wp();

	set_seconds(sec); set_minutes(min); set_hours(hour);
	set_days(day); set_months(mon); set_years(year);
      }
    }
  }
}
