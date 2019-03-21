import QtQuick 2.2

import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents


Rectangle {
    id: root
    property string cw_type
    implicitHeight: gv.cellWidth
        /* + units.smallSpacing */
    color: "transparent"

    ListModel {
        id: colorModel
        /* objectName: "gv_model" */

        /* ListElement { */
        /*     name: "Apple" */
        /*     c: "red" */
        /* } */
        /* ListElement { */
        /*     name: "Orange" */
        /*     c: "orange" */
        /* } */
        /* ListElement { */
        /*     name: "own" */
        /*     c: "yellow" */
        /* } */

    }

    Component {
        id: colorDelegate
        ColorUnit {
            cu_index: index
            inner_color: c
            c_name: name
        }
    }

    /* ColorUnit { */
    /*     id: colorDelegate */
    /*     cu_index: index */
    /*     inner_color: c */
    /*     c_name: name */
    /* } */

    GridView {
        id: gv
        objectName: "grid_view"
        /* Layout.alignment: Qt.AlignCenter */
        signal cu_clicked(string color)

        anchors {
            fill: parent
        }
        cellWidth: units.iconSizes.medium + units.smallSpacing
        cellHeight: cellWidth
        /* anchors.fill: parent */
        model: colorModel
        delegate: colorDelegate
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0; duration: 200}
                NumberAnimation { property: "scale"; to: 0; duration: 300; easing.type: Easing.OutCubic}
            }
        }
        add: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 200}
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 300; easing.type: Easing.OutCubic}
            }
        }
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.OutCubic}
        }
        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.OutCubic}
        }

        function prepend_color(color_code) {
            if (root.cw_type == "cw1") {
                var i = 0;
                while (i < 10 && i < colorModel.count) {
                    if (colorModel.get(i).c == color_code) {
                        colorModel.move(i, 0, 1);
                        return;
                    }
                    i++;
                }
                colorModel.insert(0, {"name": "user", "c": color_code});
                while (colorModel.count > 10) {
                    colorModel.remove(i);
                }
            } else {
                colorModel.insert(0, {"name": "user", "c": color_code});
            }
        }
        function add_color(color_code) {
            var i = colorModel.count - 1;
            if (colorModel.get(i).name == "own") {
                colorModel.insert(i, {"name": "user", "c": color_code})
            } else {
                colorModel.append({"name": "user", "c": color_code})
            }
        }
        function get_colors() {
            var i = colorModel.count - 1;
            var r = "";
            while (i >= 0) {
                if (colorModel.get(i).name != "user") {
                    if (colorModel.get(i).name == "own") {
                        i--;
                        continue;
                    } else {
                        break;
                    }
                } else {
                    r = colorModel.get(i).c + "," + r;
                }
                i--;
            }
            return r;
        }
        function remove_color(index) {
            colorModel.remove(index);
        }

        Component.onCompleted: {
            if (root.cw_type == "cw2") {
                colorModel.append({"name": "own", "c": "grey"})
            }
        }
    }
}
