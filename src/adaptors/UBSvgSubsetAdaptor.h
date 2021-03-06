/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UBSVGSUBSETADAPTOR_H_
#define UBSVGSUBSETADAPTOR_H_

#include <QtGui>
#include <QtXml>

#include "frameworks/UBGeometryUtils.h"

class UBGraphicsSvgItem;
class UBGraphicsPolygonItem;
class UBGraphicsPixmapItem;
class UBGraphicsPDFItem;
class UBGraphicsWidgetItem;
class UBGraphicsVideoItem;
class UBGraphicsAudioItem;
class UBGraphicsAppleWidgetItem;
class UBGraphicsW3CWidgetItem;
class UBGraphicsTextItem;
class UBGraphicsCurtainItem;
class UBGraphicsRuler;
class UBGraphicsCompass;
class UBGraphicsProtractor;
class UBGraphicsScene;
class UBDocumentProxy;
class UBGraphicsStroke;
class UBPersistenceManager;
class UBGraphicsTriangle;
class UBGraphicsCache;

class UBSvgSubsetAdaptor
{
    private:

        UBSvgSubsetAdaptor() {;}
        virtual ~UBSvgSubsetAdaptor() {;}

    public:

        static UBGraphicsScene* loadScene(UBDocumentProxy* proxy, const int pageIndex);
        static void persistScene(UBDocumentProxy* proxy, UBGraphicsScene* pScene, const int pageIndex);
        static void upgradeScene(UBDocumentProxy* proxy, const int pageIndex);

        static QUuid sceneUuid(UBDocumentProxy* proxy, const int pageIndex);
        static void setSceneUuid(UBDocumentProxy* proxy, const int pageIndex, QUuid pUuid);

        static void convertPDFObjectsToImages(UBDocumentProxy* proxy);
        static void convertSvgImagesToImages(UBDocumentProxy* proxy);

        static const QString nsSvg;
        static const QString nsXLink;
        static const QString nsXHtml;
        static const QString nsUb;
        static const QString xmlTrue;
        static const QString xmlFalse;

        static const QString sFontSizePrefix;
        static const QString sPixelUnit;
        static const QString sFontWeightPrefix;
        static const QString sFontStylePrefix;

    private:

        static UBGraphicsScene* loadScene(UBDocumentProxy* proxy, const QByteArray& pArray);

        static QDomDocument loadSceneDocument(UBDocumentProxy* proxy, const int pPageIndex);

        static QString uniboardDocumentNamespaceUriFromVersion(int fileVersion);

        static const QString sFormerUniboardDocumentNamespaceUri;

        static QString toSvgTransform(const QMatrix& matrix);
        static QMatrix fromSvgTransform(const QString& transform);

        class UBSvgSubsetReader
        {
            public:

                UBSvgSubsetReader(UBDocumentProxy* proxy, const QByteArray& pXmlData);

                virtual ~UBSvgSubsetReader(){}

                UBGraphicsScene* loadScene();

            private:

                UBGraphicsPolygonItem* polygonItemFromLineSvg(const QColor& pDefaultBrushColor);

                UBGraphicsPolygonItem* polygonItemFromPolygonSvg(const QColor& pDefaultBrushColor);

                QList<UBGraphicsPolygonItem*> polygonItemsFromPolylineSvg(const QColor& pDefaultColor);

                UBGraphicsPixmapItem* pixmapItemFromSvg();

                UBGraphicsSvgItem* svgItemFromSvg();

                UBGraphicsPDFItem* pdfItemFromPDF();

                UBGraphicsVideoItem* videoItemFromSvg();

                UBGraphicsAudioItem* audioItemFromSvg();

                UBGraphicsAppleWidgetItem* graphicsAppleWidgetFromSvg();

                UBGraphicsW3CWidgetItem* graphicsW3CWidgetFromSvg();

                UBGraphicsTextItem* textItemFromSvg();

                UBGraphicsCurtainItem* curtainItemFromSvg();

                UBGraphicsRuler* rulerFromSvg();

                UBGraphicsCompass* compassFromSvg();

                UBGraphicsProtractor* protractorFromSvg();

                UBGraphicsTriangle* triangleFromSvg();

                UBGraphicsCache* cacheFromSvg();

                void graphicsItemFromSvg(QGraphicsItem* gItem);

                QXmlStreamReader mXmlReader;
                int mFileVersion;
                UBDocumentProxy *mProxy;
                QString mDocumentPath;

                QColor mGroupDarkBackgroundColor;
                QColor mGroupLightBackgroundColor;
                qreal mGroupZIndex;
                bool mGroupHasInfo;

                QString mNamespaceUri;
        };

        class UBSvgSubsetWriter
        {
            public:

                UBSvgSubsetWriter(UBDocumentProxy* proxy, UBGraphicsScene* pScene, const int pageIndex);

                bool persistScene();

                virtual ~UBSvgSubsetWriter(){}

            private:

                void polygonItemToSvgPolygon(UBGraphicsPolygonItem* polygonItem, bool groupHoldsInfo);
                void polygonItemToSvgLine(UBGraphicsPolygonItem* polygonItem, bool groupHoldsInfo);
                void strokeToSvgPolyline(UBGraphicsStroke* stroke, bool groupHoldsInfo);
                void strokeToSvgPolygon(UBGraphicsStroke* stroke, bool groupHoldsInfo);

                inline QString pointsToSvgPointsAttribute(const QVector<QPointF> points)
                {
                    const QVector<QPointF> crashedPoints = UBGeometryUtils::crashPointList(points);

                    int pointsCount = crashedPoints.size();
                    QString svgPoints;

                    static const int maxBufferLengthPerPoint = 30;
                    int maxBufferLength = maxBufferLengthPerPoint * pointsCount;
                    char *buffer = new char[maxBufferLength];
                    int length = 0;

                    for(int j = 0; j < pointsCount; j++)
                    {
                            const QPointF & point = crashedPoints.at(j);
                            int n = sprintf(buffer + length, "%.2f,%.2f ", point.x(), point.y());
                            length += n;
                    }

                    svgPoints = QString::fromAscii(buffer, length);
                    delete buffer;
                    return svgPoints;
                }

                inline qreal trickAlpha(qreal alpha)
                {
                        qreal trickAlpha = alpha;
                        if(trickAlpha >= 0.2 && trickAlpha < 0.6){
                                trickAlpha /= 5;
                        }else if (trickAlpha < 0.8)
                            trickAlpha /= 3;

                        return trickAlpha;
                }

                void pixmapItemToLinkedImage(UBGraphicsPixmapItem *pixmapItem);
                void svgItemToLinkedSvg(UBGraphicsSvgItem *svgItem);
                void pdfItemToLinkedPDF(UBGraphicsPDFItem *pdfItem);
                void videoItemToLinkedVideo(UBGraphicsVideoItem *videoItem);
                void audioItemToLinkedAudio(UBGraphicsAudioItem* audioItem);
                void graphicsItemToSvg(QGraphicsItem *item);
                void graphicsAppleWidgetToSvg(UBGraphicsAppleWidgetItem *item);
                void graphicsW3CWidgetToSvg(UBGraphicsW3CWidgetItem *item);
                void graphicsWidgetToSvg(UBGraphicsWidgetItem *item);
                void textItemToSvg(UBGraphicsTextItem *item);
                void curtainItemToSvg(UBGraphicsCurtainItem *item);
                void rulerToSvg(UBGraphicsRuler *item);
                void compassToSvg(UBGraphicsCompass *item);
                void protractorToSvg(UBGraphicsProtractor *item);
                void cacheToSvg(UBGraphicsCache* item);
                void triangleToSvg(UBGraphicsTriangle *item);
                void writeSvgElement();

        private:

                UBGraphicsScene* mScene;
                QXmlStreamWriter mXmlWriter;
                QString mDocumentPath;
                int mPageIndex;

        };
};

#endif /* UBSVGSUBSETADAPTOR_H_ */
