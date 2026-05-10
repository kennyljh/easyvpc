#ifndef GUIUTIL_H
#define GUIUTIL_H

#include <QObject>
#include <QWidget>

class GUIUtil {
    public:
        GUIUtil();

        /**
         * @brief applyWidgetFade - adds fade animation to selected widget with
         * desired duration
         * @param widget
         * @param duration
         */
        static void applyWidgetFade(QWidget *widget, int duration);
};

#endif // GUIUTIL_H
