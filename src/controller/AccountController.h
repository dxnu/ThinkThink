#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include <QObject>
#include <AccountModel.h>



class AccountController : public QObject
{
    Q_OBJECT
public:
    explicit AccountController(QObject *parent = nullptr);

    Q_INVOKABLE AccountModel* model() const;

    Q_INVOKABLE void account_id(const QString& id);
    Q_INVOKABLE void account_name(const QString& name);
    Q_INVOKABLE void password(const QString& password);
    Q_INVOKABLE void mail(const QString& mail);
    Q_INVOKABLE void profile_photo(const QString& profile_photo);
    Q_INVOKABLE void gender(const QString& gender);
    Q_INVOKABLE void about_me(const QString& about_me);

    Q_INVOKABLE void registerAccount();
    Q_INVOKABLE void login(const QString& id, const QString& password);

    Q_INVOKABLE void getVerifyCode(const QString& mail);
    Q_INVOKABLE void getResetPasswordVerifyCode(const QString& account);
    Q_INVOKABLE bool checkMail(const QString& mail);

    Q_INVOKABLE QString getUserId() const;
    Q_INVOKABLE QString getProfileImage() const;

    Q_INVOKABLE void changePassword();

    Q_INVOKABLE void logged(bool flag);
    Q_INVOKABLE bool logged() const;

    Q_INVOKABLE void updateInfo() const;

private:
    AccountModel* m_accountModel;
    bool m_logged{false};
};

#endif // ACCOUNTCONTROLLER_H
