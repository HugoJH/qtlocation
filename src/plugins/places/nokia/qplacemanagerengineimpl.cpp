#include "qplacemanagerengineimpl.h"

#include "qplacecategoriesrepository.h"
#include "qplacetextpredictionreplyimpl.h"
#include "qplacesearchreplyimpl.h"
#include "qplacereviewreplyimpl.h"
#include "qplacemediareplyimpl.h"
#include "qplacerecommendationreplyimpl.h"
#include "qplacedetailsreplyimpl.h"
#include "qplaceratingreplyimpl.h"
#include "qplacerestmanager.h"
#include "qplacerestreply.h"

QTM_USE_NAMESPACE

const char *MANAGER_NAME = "com.nokia.places/0.9";

QPlaceManagerEngineImpl::QPlaceManagerEngineImpl(QObject *parent)
    : QPlaceManagerEngine(parent)
{
}

QPlaceManagerEngineImpl::~QPlaceManagerEngineImpl()
{
}

QString QPlaceManagerEngineImpl::managerName() const
{
    return MANAGER_NAME;
}

QPlaceDetailsReply *QPlaceManagerEngineImpl::getPlaceDetails(const QString &placeId)
{
    QPlaceDetailsReplyImpl *reply = NULL;
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendPlaceRequest(placeId);
    if (restReply) {
        reply = new QPlaceDetailsReplyImpl(restReply, this);
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceMediaReply *QPlaceManagerEngineImpl::getMedia(const QPlace &place, const QPlaceQuery &query)
{
    QPlaceMediaReplyImpl *reply = NULL;
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendPlaceImagesRequest(place.placeId(),
                                                                                       query);
    if (restReply) {
        reply = new QPlaceMediaReplyImpl(restReply, this);
        reply->setStartNumber(query.offset());
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceReply *QPlaceManagerEngineImpl::postRating(const QPlace &place, qreal value)
{
    QPlaceRatingReplyImpl *reply = NULL;
    //TODO: need to post user name
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->postRatingRequest(place.placeId(),
                                                                                  QString(),
                                                                                  value);
    if (restReply) {
        reply = new QPlaceRatingReplyImpl(restReply, this);
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceReviewReply *QPlaceManagerEngineImpl::getReviews(const QPlace &place, const QPlaceQuery &query)
{
    QPlaceReviewReplyImpl *reply = NULL;
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendPlaceReviewRequest(place.placeId(),
                                                                                       query);
    if (restReply) {
        reply = new QPlaceReviewReplyImpl(restReply, this);
        reply->setStartNumber(query.offset());
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceSearchReply *QPlaceManagerEngineImpl::searchForPlaces(const QPlaceSearchQuery &query, QPlaceManager::VisibilityScope scope)
{
    //TODO: handling of scope
    QPlaceSearchReplyImpl *reply = NULL;
    QPlaceSearchQuery newQuery = query;
    // search by category
    if (newQuery.categories().count()) {
        newQuery.setSearchTerm(query.categories().at(0).categoryId());
    }
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendSearchRequest(newQuery);

    if (restReply) {
        reply = new QPlaceSearchReplyImpl(restReply, this);
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceManager::VisibilityScopes QPlaceManagerEngineImpl::supportedSearchVisibilityScopes() const
{
    return QPlaceManager::PublicScope;
}

QPlaceSearchReply *QPlaceManagerEngineImpl::recommendations(const QPlace &place, const QPlaceSearchQuery &query)
{
    QPlaceRecommendationReplyImpl *reply = NULL;
    QPlaceSearchQuery newQuery = query;
    newQuery.setSearchTerm(place.placeId());
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendRecommendationRequest(newQuery, QString());
    if (restReply) {
        reply = new QPlaceRecommendationReplyImpl(restReply, this);
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceTextPredictionReply *QPlaceManagerEngineImpl::textPredictions(const QPlaceSearchQuery &query)
{
    QPlaceTextPreditionReplyImpl *reply = NULL;
    QPlaceRestReply *restReply = QPlaceRestManager::instance()->sendSuggestionRequest(query);
    if (restReply) {
        reply = new QPlaceTextPreditionReplyImpl(restReply, this);
        connect(reply, SIGNAL(processingError(QPlaceReply*,QPlaceReply::Error,QString)),
                this, SLOT(processingError(QPlaceReply*,QPlaceReply::Error,QString)));
        connect(reply, SIGNAL(processingFinished(QPlaceReply*)),
                this, SLOT(processingFinished(QPlaceReply*)));
    }
    return reply;
}

QPlaceManager::ConnectivityModes QPlaceManagerEngineImpl::connectivityMode() const
{
    return QPlaceManager::OnlineMode;
}

void QPlaceManagerEngineImpl::setConnectivityMode(QPlaceManager::ConnectivityModes mode)
{
    Q_UNUSED(mode)
    // only QPlaceManager::OnlineMode is suppoerted
}

QPlaceManager::ConnectivityModes QPlaceManagerEngineImpl::supportedConnectivityModes() const
{
    return QPlaceManager::OnlineMode;
}

QPlaceReply *QPlaceManagerEngineImpl::savePlace(QPlace *place, QPlaceManager::VisibilityScope scope) {
    //TODO: implementation
    return NULL;
}

QPlaceManager::VisibilityScopes QPlaceManagerEngineImpl::supportedSaveVisibilityScopes() const {
    return QPlaceManager::NoScope;
}

QPlaceReply *QPlaceManagerEngineImpl::removePlace(const QPlace &place) {
    //TODO: implementation
    return NULL;
}

QPlaceReply *QPlaceManagerEngineImpl::initializeCategories()
{
    return QPlaceCategoriesRepository::instance()->initializeCategories();
}

QList<QPlaceCategory> QPlaceManagerEngineImpl::categories() const
{
    return QPlaceCategoriesRepository::instance()->categories();
}

void QPlaceManagerEngineImpl::processingError(QPlaceReply *reply,
                                              const QPlaceReply::Error &errorId,
                                              const QString &errorMessage)
{
    emit error(reply, errorId, errorMessage);
}

void QPlaceManagerEngineImpl::processingFinished(QPlaceReply *reply)
{
    emit finished(reply);
}
