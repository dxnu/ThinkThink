#include "AccountModel.h"
#include "qdebug.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <regex>


AccountModel::AccountModel(QObject *parent)
    : QObject{parent}
{
    m_manager = new QNetworkAccessManager();
    connect(m_manager, &QNetworkAccessManager::finished, this, &AccountModel::managerFinished);
}

void AccountModel::account_id(const QString &id)
{
    m_account.account_id = id;
}

void AccountModel::account_name(const QString &name)
{
    m_account.account_name = name;
}

void AccountModel::password(const QString &password)
{
    m_account.password_hash = password;
}

void AccountModel::mail(const QString &mail)
{
    m_account.mail = mail;
}

void AccountModel::profile_photo(const QString &profile_photo)
{
    m_account.profile_photo = profile_photo;

    qDebug() << "set profile image: " << m_account.profile_photo;
}

void AccountModel::gender(const QString &gender)
{
    m_account.gender = gender;
}

void AccountModel::about_me(const QString &about_me)
{
    m_account.about_me = about_me;
}


void AccountModel::registerAccount()
{
    qDebug() << "account_id: " << m_account.account_id << '\n'
             << "account_name: " << m_account.account_name << '\n'
             << "password: " << m_account.password_hash << '\n'
             << "about_me: " << m_account.about_me << '\n'
             << "mail: " << m_account.mail;

    QString data = QString("http://127.0.0.1/register_account?account_id=%1&account_name=%2&password=%3&mail=%4&about_me=%5")
            .arg(m_account.account_id).arg(m_account.account_name).arg(m_account.password_hash)
            .arg(m_account.mail).arg(m_account.about_me);
    m_manager->get(QNetworkRequest(QUrl(data)));
}

void AccountModel::changePassword()
{
    qDebug() << "change password";

    QString data = QString("http://127.0.0.1/change_password?account_id=%1&password=%2")
            .arg(m_account.account_id).arg(m_account.password_hash);
    m_manager->get(QNetworkRequest(QUrl(data)));
}

void AccountModel::login(const QString &id, const QString &password)
{
    QString data = QString("http://127.0.0.1/login?account_id=%1&password=%2").arg(id).arg(password);
    m_manager->get(QNetworkRequest(QUrl(data)));
}

void AccountModel::getVerifyCode(const QString &mail)
{
    QString url = QString("http://127.0.0.1/get_verify_code?mail=%1").arg(mail);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void AccountModel::getResetPasswordVerifyCode(const QString &account)
{
    QString url = QString("http://127.0.0.1/get_reset_password_verify_code?account_id=%1").arg(account);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

bool AccountModel::checkMail(const QString &mail)
{
    std::regex reg("[a-zA-Z0-9_]+@[a-zA-Z0-9]+(\\.[a-zA-Z]+){1,3}");
    return std::regex_match(mail.toStdString(), reg);
}

QString AccountModel::getUserId() const
{
    return m_account.account_id;
}

QString AccountModel::getProfileImage() const
{
    return m_account.profile_photo;
}

void AccountModel::updateInfo() const
{
    qDebug() << "account_id: " << m_account.account_id << '\n'
             << "account_name: " << m_account.account_name << '\n'
             << "about_me: " << m_account.about_me << '\n'
             << "mail: " << m_account.mail << '\n'
             << "gender: " << m_account.gender;
    QString url = QString("http://127.0.0.1/update_info?id=%1&name=%2&gender=%3&about_me=%4")
            .arg(m_account.account_id).arg(m_account.account_name).arg(m_account.gender).arg(m_account.about_me);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void AccountModel::managerFinished(QNetworkReply *reply)
{
    qDebug() << "managerFinished";

    QString response = QString::fromLocal8Bit(reply->readAll());

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "ResponseType")
                {
                    QString value = xml.readElementText();
                    qDebug() << "response type" << value;
                    if(value == "RegisterAccount") OnRegisterAccount(response);
                    if(value == "GetVerificationCode") OnGetVerificationCode(response);
                    if(value == "Login") OnLogin(response);
                    if(value == "GetResetPasswordVerficationCode") OnGetResetPasswordVerficationCode(response);
                    if(value == "ChangePassword") OnChangePassword(response);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }
}

void AccountModel::OnRegisterAccount(const QString &response)
{
    bool status{false};
    QString errorMsg;

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "Status")
                {
                    QString value = xml.readElementText();
                    if(value == "OK") status = true;
                    else status = false;
                    qDebug() << "status: " << value;
                }
                else if(xml.name().toString() == "ErrorMsg")
                {
                    errorMsg = xml.readElementText();
                    qDebug() << "lastError: " << errorMsg;
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }

    emit registerAccountReply(status, errorMsg);
}

void AccountModel::OnGetVerificationCode(const QString &response)
{
    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "VerificationCode")
                {
                    QString value = xml.readElementText();
                    qDebug() << "verfication Code: " << value;
                    emit verificationCodeChanged(value);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }
}

void AccountModel::OnGetResetPasswordVerficationCode(const QString &response)
{
    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "VerificationCode")
                {
                    QString value = xml.readElementText();
                    qDebug() << "reset password verfication Code: " << value;
                    emit resetPasswordVerificationCodeChanged(value);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }
}

void AccountModel::OnLogin(const QString &response)
{
    bool loginStatus{false};
    QString loginMessage;
    QString loginName;
    QString loginProfileImage{"http://127.0.0.1/"};

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "LoginStatus")
                {
                    QString value = xml.readElementText();
                    if(value == "OK") loginStatus = true;
                    qDebug() << "login status: " << value;
                }
                else if(xml.name().toString() == "LoginMessage")
                {
                    loginMessage = xml.readElementText();
                    qDebug() << "login message: " << loginMessage;

                }
                else if(xml.name().toString() == "LoginName")
                {
                    loginName = xml.readElementText();
                    m_account.account_name = xml.readElementText();
                }
                else if(xml.name().toString() == "LoginAvatar")
                {
                    loginProfileImage += xml.readElementText();
                    m_account.profile_photo = xml.readElementText();
                }
                else if(xml.name().toString() == "AccountId")
                {
                    m_account.account_id = xml.readElementText();
                }
                else if(xml.name().toString() == "Mail")
                {
                    m_account.mail = xml.readElementText();
                }
                else if(xml.name().toString() == "Gender")
                {
                    m_account.gender = xml.readElementText();
                }
                else if(xml.name().toString() == "AboutMe")
                {
                    m_account.about_me = xml.readElementText();
                }
                else if(xml.name().toString() == "SignupDate")
                {
                    m_account.signup_date = xml.readElementText();
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }

    emit loginResponse(loginStatus, loginMessage, loginName, loginProfileImage);

    if(loginStatus)
        emit initUserInfo(m_account.account_id, m_account.account_name,
                          m_account.mail, m_account.gender, m_account.about_me, m_account.profile_photo, m_account.signup_date);
}

void AccountModel::OnChangePassword(const QString &response)
{
    bool status{false};
    QString errorMsg;

    QXmlStreamReader xml(response);
    if(xml.readNextStartElement())
    {
        if(xml.name().toString() == "ThinkThink") // 确保是ThinkThink消息
        {
            while(xml.readNextStartElement())
            {
                if(xml.name().toString() == "Status")
                {
                    QString value = xml.readElementText();
                    if(value == "OK") status = true;
                    else status = false;
                    qDebug() << "status: " << value;
                }
                else if(xml.name().toString() == "ErrorMsg")
                {
                    errorMsg = xml.readElementText();
                    qDebug() << "lastError: " << errorMsg;
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.raiseError(QObject::tr("invalid response message"));
        }
    }

    emit changePasswordReply(status, errorMsg);
}
