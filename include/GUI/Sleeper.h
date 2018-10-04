/* 
 * File:   Sleeper.h
 * Author: christian
 *
 * Created on April 18, 2016, 12:39 AM
 */

#ifndef SLEEPER_H
#define	SLEEPER_H

#include <QThread>    

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

#endif	/* SLEEPER_H */

