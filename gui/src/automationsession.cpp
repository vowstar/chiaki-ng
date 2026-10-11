// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL
#include "qmlbackend.h"

QString QmlBackend::automationSessionCommand(const QString &command, int index)
{
	if(command == QLatin1String("disconnect"))
	{
		stopAutoConnect();
		stopSession(false);
		return {};
	}
	if(index < 0)
		return QStringLiteral("host must be a nonnegative integer index from hosts");
	if(command == QLatin1String("connect") && session)
		return {};
	const auto server = displayServerAt(index);
	if(!server.valid)
		return QStringLiteral("host index is no longer available, query hosts again");
	if(!server.registered)
		return QStringLiteral("host must be registered in the GUI first");
	if(!server.duid.isEmpty())
		return QStringLiteral("session control requires a local or manual host");
	if(command == QLatin1String("wakeup"))
		return sendWakeup(server) ? QString() : QStringLiteral("failed to send wakeup packet");
	const QString nickname = server.discovered ? server.discovery_host.host_name : server.registered_host.GetServerNickname();
	connectToHost(index, nickname);
	return session ? QString() : QStringLiteral("failed to start session");
}
