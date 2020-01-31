/****************************************************************************
 *
 *   (c) 2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QColor>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QGeoRectangle>
#include <QVariant>

#include <memory>

class MGRSLabel
{
public:
    MGRSLabel(const QString& label, const QGeoCoordinate& pos, const QString& foregroundColor, const QString& backgroundColor)
    {
        _label = label;
        _pos = pos;
        _foregroundColor = foregroundColor;
        _backgroundColor = backgroundColor;
    }

    QString _label;
    QGeoCoordinate _pos;
    QString _foregroundColor;
    QString _backgroundColor;
};

class MGRSZone
{
public:
    MGRSZone(QString _label);

    bool visited = false;
    bool valid = true;
    bool rightOverlap = false;
    QString label;
    QGeoCoordinate bottomLeft;
    QGeoCoordinate upperLeft;
    QGeoCoordinate bottomRight;
    QGeoCoordinate upperRight;
    QGeoCoordinate searchPos;
    QGeoCoordinate rightSearchPos;
    QGeoCoordinate leftSearchPos;
    QGeoCoordinate topSearchPos;
    QGeoCoordinate bottomSearchPos;
};

class MapGridMGRS : public QObject
{
    Q_OBJECT
public:
    explicit MapGridMGRS();

    static bool lineIntersectsLine(const QGeoCoordinate& l1p1, const QGeoCoordinate& l1p2, const QGeoCoordinate& l2p1, const QGeoCoordinate& l2p2);

    static bool lineIntersectsRect(const QGeoCoordinate& p1, const QGeoCoordinate& p2, const QGeoRectangle& r);

    static QString level1Label(QString mgrs) { return mgrs.left(3); }

    static QString zoneLabel(QString mgrs) { return mgrs.left(5); }

public slots:
    void geometryChanged(double zoomLevel, const QGeoCoordinate& topLeft, const QGeoCoordinate& topRight, const QGeoCoordinate& bottomLeft, const QGeoCoordinate& bottomRight);

signals:
    void updateValues(const QVariant& values);

private:
    const double maxZoneZoomLevel = 7.5;

    const QString level1LineForegroundColor = "#AAFF9999";
    const int level1LineForgroundWidth = 3;
    const QString level1LineBackgroundColor = "#55660000";
    const int level1LineBackgroundWidth = 5;
    const QString level1LabelForegroundColor = "#AAFF0000";
    const QString level1LabelBackgroundColor = "#44FFFFFF";

    const QString level2LineForegroundColor = "#AAb3FFb3";
    const int level2LineForgroundWidth = 2;
    const QString level2LineBackgroundColor = "#55006600";
    const int level2LineBackgroundWidth = 4;
    const QString level2LabelForegroundColor = "#AA00FF00";
    const QString level2LabelBackgroundColor = "#44FFFFFF";

    const QString level3LineForegroundColor = "#77FFFFFF";
    const int level3LineForgroundWidth = 1;
    const QString level3LineBackgroundColor = "#44000000";
    const int level3LineBackgroundWidth = 3;
    const QString level3LabelForegroundColor = "#AAFF0000";
    const QString level3LabelBackgroundColor = "#44FFFFFF";

    QGeoRectangle _currentMGRSRect;
    double _zoomLevel;
    QMap<QString, std::shared_ptr<MGRSZone>> _zoneMap;
    QList<QGeoPath> _level1lines;
    QList<MGRSLabel> _level1labels;
    QList<QGeoPath> _level1Paths;
    QList<QGeoPath> _level2Paths;
    QList<QGeoPath> _level3Paths;
    QList<MGRSLabel> _mgrsLabels;

    void _clear();

    bool _zoomLevelCrossed(double newZoomLevel, double prevZoomLevel);

    void _addLevel1Lines();

    void _addLevel1Labels();

    void _findZoneBoundaries(const QGeoCoordinate& start);

    void _addLines(QJsonArray& lines, const QList<QGeoPath>& paths, const QString& color, int width);

    void _addLabels(QJsonArray& labels);
};
