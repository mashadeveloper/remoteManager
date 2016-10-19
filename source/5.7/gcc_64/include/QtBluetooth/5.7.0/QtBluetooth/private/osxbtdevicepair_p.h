/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef OSXBTDEVICEPAIR_P_H
#define OSXBTDEVICEPAIR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qbluetoothaddress.h"
#include "osxbtutility_p.h"

#include <QtCore/qglobal.h>

#include <Foundation/Foundation.h>
// Only after Foundation.h:
#include "corebluetoothwrapper_p.h"

@class QT_MANGLE_NAMESPACE(OSXBTPairing);
@class IOBluetoothDevice;

QT_BEGIN_NAMESPACE

namespace OSXBluetooth {

// C++ delegate.
class PairingDelegate
{
public:
    typedef QT_MANGLE_NAMESPACE(OSXBTPairing) ObjCPairingRequest;

    virtual ~PairingDelegate();

    virtual void connecting(ObjCPairingRequest *pair) = 0;
    virtual void requestPIN(ObjCPairingRequest *pair) = 0;
    virtual void requestUserConfirmation(ObjCPairingRequest *pair,
                                         BluetoothNumericValue) = 0;
    virtual void passkeyNotification(ObjCPairingRequest *pair,
                                     BluetoothPasskey passkey) = 0;
    virtual void error(ObjCPairingRequest *pair, IOReturn errorCode) = 0;
    virtual void pairingFinished(ObjCPairingRequest *pair) = 0;
};

ObjCStrongReference<IOBluetoothDevice> device_with_address(const QBluetoothAddress &address);

} // Namespace OSXBluetooth.

QT_END_NAMESPACE

@interface QT_MANGLE_NAMESPACE(OSXBTPairing) : NSObject<IOBluetoothDevicePairDelegate>
{
    QT_PREPEND_NAMESPACE(QBluetoothAddress) m_targetAddress;

    bool m_active;
    IOBluetoothDevicePair *m_pairing; // The real pairing request
    QT_PREPEND_NAMESPACE(OSXBluetooth)::PairingDelegate *m_object;
}

- (id)initWithTarget:(const QBluetoothAddress &)address
      delegate:(QT_PREPEND_NAMESPACE(OSXBluetooth::PairingDelegate) *)object;

- (void)dealloc;

- (IOReturn)start;
- (bool)isActive;
- (void)stop;

- (const QBluetoothAddress &)targetAddress;
- (IOBluetoothDevicePair *)pairingRequest;
- (IOBluetoothDevice *)targetDevice;

// IOBluetoothDevicePairDelegate:

- (void)devicePairingStarted:(id)sender;
- (void)devicePairingConnecting:(id)sender;
- (void)deviceParingPINCodeRequest:(id)sender;

- (void)devicePairingUserConfirmationRequest:(id)sender
        numericValue:(BluetoothNumericValue)numericValue;

- (void)devicePairingUserPasskeyNotification:(id)sender
        passkey:(BluetoothPasskey)passkey;

- (void)devicePairingFinished:(id)sender
        error:(IOReturn)error;

- (void)deviceSimplePairingComplete:(id)sender
        status:(BluetoothHCIEventStatus)status;

@end


#endif
