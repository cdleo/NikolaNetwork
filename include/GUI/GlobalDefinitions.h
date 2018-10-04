/* 
 * File:   GlobalDefinitions.h
 * Author: cleo
 *
 * Created on August 5, 2014, 9:20 AM
 */

#ifndef GUIGLOBALDEFINITIONS_H
#define	GUIGLOBALDEFINITIONS_H

#include <map>
#include <vector>
#include <string>
#include <qnamespace.h>

typedef enum DigitalCapabilityStatus {
    DigitalCapabilityStatus_Unknown = 0,
    DigitalCapabilityStatus_On = 1,
    DigitalCapabilityStatus_Off = 2
} DigitalCapabilityStatus;

typedef enum AnalogCapabilityStatus {
    AnalogCapabilityStatus_Unknown = 0,
    AnalogCapabilityStatus_WithValue = 1
} AnalogCapabilityStatus;

typedef enum WidgetTypes {
    WidgetType_Unknown = 0,
    WidgetType_PushButton = 1,
    WidgetType_ToggleButton = 2,
    WidgetType_IndicatorButton = 3,
    WidgetType_LedIndicator = 4,
    WidgetType_Slider = 5,            
    WidgetType_AnalogValue = 6,
    WidgetType_GenericList = 7,
    WidgetType_CapabilitiesList = 8
} WidgetTypes;

typedef enum TransportType {
    TransportType_Undefined = 0,
    TransportType_Widget = 1,
    TransportType_AnalogValue = 2,
    TransportType_DigitalValue = 3
} TransportType;

typedef enum NWColor {
    NW_Color0 = Qt::color0,
    NW_Color1 = Qt::color1,
    NW_Black = Qt::black,
    NW_White = Qt::white,
    NW_DarkGray = Qt::darkGray,
    NW_Gray = Qt::gray,
    NW_LightGray = Qt::lightGray,
    NW_Red = Qt::red,
    NW_Green = Qt::green,
    NW_Blue = Qt::blue,
    NW_Cyan = Qt::cyan,
    NW_Magenta = Qt::magenta,
    NW_Yellow = Qt::yellow,
    NW_DarkRed = Qt::darkRed,
    NW_DarkGreen = Qt::darkGreen,
    NW_DarkBlue = Qt::darkBlue,
    NW_DarkCyan = Qt::darkCyan,
    NW_DarkMagenta = Qt::darkMagenta,
    NW_DarkYellow = Qt::darkYellow,
    NW_Transparent = Qt::transparent,
    NW_Orange,
    NW_Violet
} NWColor;

#endif	/* GUIGLOBALDEFINITIONS_H */

