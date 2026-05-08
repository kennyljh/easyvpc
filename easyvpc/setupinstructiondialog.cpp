#include "setupinstructiondialog.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

SetupInstructionDialog::SetupInstructionDialog(QWidget *parent)
                       : QDialog(parent)
{
    setWindowTitle("EasyVPC Setup Instructions");

    resize(900, 650);

    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *container = new QWidget(scrollArea);

    QVBoxLayout *containerLayout =
        new QVBoxLayout(container);

    containerLayout->setContentsMargins(20, 20, 20, 20);
    containerLayout->setSpacing(15);

    QLabel *instructions = new QLabel(container);

    instructions->setWordWrap(true);
    instructions->setTextFormat(Qt::RichText);
    instructions->setOpenExternalLinks(true);
    instructions->setTextInteractionFlags(
        Qt::TextSelectableByMouse |
        Qt::LinksAccessibleByMouse
    );

    instructions->setStyleSheet(R"(
        QLabel {
            font-size: 14px;
            line-height: 1.5;
        }

        h1 {
            font-size: 24px;
            font-weight: bold;
        }

        h2 {
            font-size: 18px;
            font-weight: bold;
            margin-top: 15px;
        }

        pre {
            background-color: #1e1e1e;
            color: #dcdcdc;
            padding: 10px;
            border-radius: 6px;
            font-family: monospace;
        }
    )");

    instructions->setText(R"(
        <h1>EasyVPC Setup Guide</h1>

        <p>
        EasyVPC uses AWS IAM Identity Center (AWS SSO)
        for authentication.
        </p>

        <p>
        Before using the application, you must:
        </p>

        <ul>
        <li>Install AWS CLI v2</li>
        <li>Configure an AWS SSO profile</li>
        <li>Login using AWS SSO</li>
        </ul>

        <hr>

        <h2>1. Install AWS CLI</h2>

        <p>
        Ubuntu / Debian:
        </p>

        <pre>
        sudo apt update
        sudo apt install unzip curl -y

        curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
        unzip awscliv2.zip
        sudo ./aws/install
        </pre>

        <p>
        Verify installation:
        </p>

        <pre>
        aws --version
        </pre>

        <p>
        Expected output:
        </p>

        <pre>
        aws-cli/2.x.x
        </pre>

        <hr>

        <h2>2. Obtain AWS IAM Identity Center Information</h2>

        <p>
        Request the following information from your AWS administrator:
        </p>

        <ul>
        <li>SSO Start URL</li>
        <li>SSO Region</li>
        <li>AWS Account ID</li>
        <li>Permission Set / Role Name</li>
        </ul>

        <p>
        Example:
        </p>

        <pre>
        SSO Start URL:
        https://example.awsapps.com/start

        SSO Region:
        us-east-1
        </pre>

        <hr>

        <h2>3. Configure AWS SSO Profile</h2>

        <p>
        Run:
        </p>

        <pre>
        aws configure sso
        </pre>

        <p>
        You will be prompted for:
        </p>

        <pre>
        SSO session name
        SSO start URL
        SSO region
        AWS account
        Permission set
        Default region
        Profile name
        </pre>

        <p>
        Example:
        </p>

        <pre>
        Profile name: production
        </pre>

        <hr>

        <h2>4. Login Using AWS SSO</h2>

        <p>
        Run:
        </p>

        <pre>
        aws sso login --profile production
        </pre>

        <p>
        Your browser should open automatically.
        </p>

        <p>
        Complete the AWS login flow.
        </p>

        <hr>

        <h2>5. Launch EasyVPC</h2>

        <ol>
        <li>Open EasyVPC</li>
        <li>Select your AWS profile</li>
        <li>Select region</li>
        <li>Click Login</li>
        </ol>

        <hr>

        <h2>Important Notes</h2>

        <p>
        AWS SSO sessions expire periodically.
        </p>

        <p>
        If authentication stops working, run:
        </p>

        <pre>
        aws sso login --profile &lt;profile-name&gt;
        </pre>

        <p>
        again to refresh your credentials.
        </p>

        <hr>

        <h2>Helpful Links</h2>

        <p>
        <a href="https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html">
        AWS CLI Installation Guide
        </a>
        </p>

        <p>
        <a href="https://docs.aws.amazon.com/cli/latest/userguide/cli-configure-sso.html">
        AWS SSO Configuration Guide
        </a>
        </p>
    )");

    containerLayout->addWidget(instructions);

    QPushButton *closeBtn =
        new QPushButton("Close", container);

    closeBtn->setMinimumHeight(36);

    containerLayout->addWidget(closeBtn);

    connect(closeBtn,
            &QPushButton::clicked,
            this,
            &QDialog::accept);

    scrollArea->setWidget(container);

    mainLayout->addWidget(scrollArea);
}
