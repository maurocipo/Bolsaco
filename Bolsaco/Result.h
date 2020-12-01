#ifndef RESULT_H
#define RESULT_H

#include <QString>

enum Status
{
    SUCCEEDED,
    FAILED,
    WARNING
};

template<typename T>
class Result
{
public:
    Result(Status aStatus, QString aError)
        : mStatus(aStatus), mError(aError) {}
    Result(T aValue)
        : mStatus(Status::SUCCEEDED), mError(""), mValue(aValue) {}

    Status status() {return mStatus;}
    QString error() {return mError;}
    T& value() {return mValue;}

private:
    Status mStatus;
    QString mError;
    T mValue;
};

template<>
class Result<void>
{
public:
    Result(Status aStatus, QString aError = "")
        : mStatus(aStatus), mError(aError) {}


    Status status() {return mStatus;}
    QString error() {return mError;}

private:
    Status mStatus;
    QString mError;
};

#endif // RESULT_H
