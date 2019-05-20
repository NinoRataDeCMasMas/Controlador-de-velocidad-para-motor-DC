/**
 * @file   Printer.h
 * @date   Abr 22, 2019
 */
#define endl "\n"
#define tab  "\t"

template<class T> inline Print &operator <<(Print &obj, T arg)
{
  obj.print(arg); return obj;
}
/* EOF */
