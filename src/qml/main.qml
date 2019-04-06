import QtQuick 2.2

import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

Rectangle {
    id: root
    implicitWidth: 500
    implicitHeight: 700
    color: PlasmaCore.ColorScope.backgroundColor

    Header {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        id: h1
        text: i18n("Wallpaper")
    }

    Wallpaper {
        anchors {
            top: h1.bottom
            left: parent.left
            right: parent.right
        }
        id: wp
    }

    Header {
        anchors {
            top: wp.bottom
            left: parent.left
            right: parent.right
        }
        id: h2
        text: i18n("Recent Colors")
    }

    ColorWidget {
        anchors {
            top: h2.bottom
            left: parent.left
            right: parent.right
        }
        id: recent_color
        cw_type: "cw1"
    }

    Header {
        anchors {
            top: recent_color.bottom
            left: parent.left
            right: parent.right
        }
        id: h3
        text: i18n("Recommended Colors")
    }

    ColorWidget {
        anchors {
            top: h3.bottom
            left: parent.left
            right: parent.right
            /* bottom: parent.bottom */
        }
        height: (units.iconSizes.medium + units.smallSpacing) * 4
        id: recommended_color
        cw_type: "cw2"
    }
}
