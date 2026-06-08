/** 
 @file functions.h
 @brief functions that preform tasks in kernel, called by commands
*/

/**
*@brief Year grab function
*
*Grabs the current year from the real time clock
*/

int kernel_get_year(void);

/**
*@brief Day grab function
*
*Grabs the current day from the real time clock
*/
int kernel_get_day(void);

/**
*@brief Month grab function
*
*Grabs the current month from the real time clock
*/
int kernel_get_month(void);

/**
*@brief Second grab function
*
*Grabs the current second from the real time clock
*/
int kernel_get_second(void);

/**
*@brief Hour grab function
*
*Grabs the current hour from the real time clock
*/
int kernel_get_hour(void);

/**
*@brief Minute grab function
*
*Grabs the current minute from the real time clock
*/
int kernel_get_minute(void);

/**
*@brief Set the second function
*
*Sets the second to the reak time clock
*/
void kernel_set_time_second(int second);

/**
*@brief Set the minute function
*
*Sets the minute to the reak time clock
*/
void kernel_set_time_minute(int minute);

/**
*@brief Set the hour function
*
*Sets the hour to the reak time clock
*/
void kernel_set_time_hour(int hour);

/**
*@brief Set the month function
*
*Sets the month to the reak time clock
*/
void kernel_set_date_month(int month);

/**
*@brief Set the day function
*
*Sets the day to the reak time clock
*/
void kernel_set_date_day(int day);

/**
*@brief Set the year function
*
*Sets the year to the reak time clock
*/
void kernel_set_date_year(int year);
