#ifndef __ERROR_HPP__
#define __ERROR_HPP__

void set_debug_mode(int mode);

int error(const char *errno, const char *message, int line_no);
void warn(const char *warnno, const char *message, int line_no);
void debug(const char *string, int level);

void check_master(struct master *ptr, int no_rows);
void check_instance(struct instance *ptr, int no_rows);

#endif // __ERROR_HPP__
