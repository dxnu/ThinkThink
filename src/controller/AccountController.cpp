#include "AccountController.h"

AccountController::AccountController(QObject *parent)
    : QObject{parent}
{
    m_accountModel = new AccountModel();
}

AccountModel *AccountController::model() const
{
   return m_accountModel;
}

void AccountController::account_id(const QString &id)
{
    m_accountModel->account_id(id);
}

void AccountController::account_name(const QString &name)
{
    m_accountModel->account_name(name);
}

void AccountController::password(const QString &password)
{
    m_accountModel->password(password);
}

void AccountController::mail(const QString &mail)
{
    m_accountModel->mail(mail);
}

void AccountController::profile_photo(const QString &profile_photo)
{
    m_accountModel->profile_photo(profile_photo);
}

void AccountController::gender(const QString &gender)
{
    m_accountModel->gender(gender);
}

void AccountController::about_me(const QString &about_me)
{
    m_accountModel->about_me(about_me);
}

void AccountController::registerAccount()
{
    m_accountModel->registerAccount();
}

void AccountController::login(const QString &id, const QString &password)
{
    m_accountModel->login(id, password);
}

void AccountController::getVerifyCode(const QString &mail)
{
    m_accountModel->getVerifyCode(mail);
}

void AccountController::getResetPasswordVerifyCode(const QString &account)
{
    m_accountModel->getResetPasswordVerifyCode(account);
}

bool AccountController::checkMail(const QString &mail)
{
    return m_accountModel->checkMail(mail);
}

QString AccountController::getUserId() const
{
    return m_accountModel->getUserId();
}

QString AccountController::getProfileImage() const
{
    return m_accountModel->getProfileImage();
}

void AccountController::changePassword()
{
    m_accountModel->changePassword();
}

void AccountController::logged(bool flag)
{
    m_logged = flag;
}

bool AccountController::logged() const
{
    return m_logged;
}

void AccountController::updateInfo() const
{
    m_accountModel->updateInfo();
}
