#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include <QObject>


class QNetworkAccessManager;
class QNetworkReply;


struct Account
{
    QString account_id;
    QString account_name;
    QString password_hash;
    QString salt;
    QString mail;
    QString profile_photo;
    QString gender;
    QString about_me;
    QString signup_date;
};


class AccountModel : public QObject
{
    Q_OBJECT
public:
    explicit AccountModel(QObject *parent = nullptr);

    void account_id(const QString& id);
    void account_name(const QString& name);
    void password(const QString& password);
    void mail(const QString& mail);
    void profile_photo(const QString& profile_photo);
    void gender(const QString& gender);
    void about_me(const QString& about_me);

    void registerAccount();
    void changePassword();

    void login(const QString& id, const QString& password);

    void getVerifyCode(const QString& mail);
    void getResetPasswordVerifyCode(const QString& account);

    bool checkMail(const QString& mail);

    // 获取当前用户ID
    QString getUserId() const;

    QString getProfileImage() const;

    void updateInfo() const;

signals:
    void registerAccountReply(bool status, const QString& errorMsg);
    void changePasswordReply(bool status, const QString& errorMsg);
    void verificationCodeChanged(const QString& code);
    void resetPasswordVerificationCodeChanged(const QString& code);
    void loginResponse(bool status, const QString& message, const QString& name, const QString& image);
    void initUserInfo(const QString& userId, const QString& userName, const QString& mail, const QString& gender, const QString& aboutMe, const QString& image, const QString& date);

private slots:
    void managerFinished(QNetworkReply *reply);

private:
    void OnRegisterAccount(const QString& response);
    void OnGetVerificationCode(const QString& response);
    void OnGetResetPasswordVerficationCode(const QString& response);
    void OnLogin(const QString& response);
    void OnChangePassword(const QString& response);

private:
    Account m_account;
    QNetworkAccessManager* m_manager;
};

#endif // ACCOUNTMODEL_H
