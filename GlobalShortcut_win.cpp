/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GlobalShortcut_win.h"
#include "MainWindow.h"

#define DX_SAMPLE_BUFFER_SIZE 512

GUID g_guidApp = { /* ab3baca6-64bc-4d32-9752-49ae9748c06e */
    0xab3baca6,
    0x64bc,
    0x4d32,
    {0x97, 0x52, 0x49, 0xae, 0x97, 0x48, 0xc0, 0x6e}
};

DIACTION g_rgGameAction[] =
{
};

#define NUMBER_OF_GAMEACTIONS    (sizeof(g_rgGameAction)/sizeof(DIACTION))

static GlobalShortcutWin *gsw = NULL;

GlobalShortcutWin::GlobalShortcutWin() {
	gsw = this;
	ref = 0;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeTicked()));

    HRESULT hr;

    ZeroMemory( &m_diafGame, sizeof(DIACTIONFORMAT) );

    m_pDI = NULL;
    m_diaActions = NULL;

    if(FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
    {
		qFatal("GlobalShortcutWin: Failed to create d8input");
        return;
	}

	qsUsername="MumbleUser";

	remap();

	timer->start(20);
}

void GlobalShortcutWin::add(GlobalShortcut *gs) {
	m_qmShortcuts[gs->idx] = gs;
	m_bNeedRemap = true;
}

void GlobalShortcutWin::remove(GlobalShortcut *gs) {
	m_qmShortcuts.remove(gs->idx);
	m_bNeedRemap = true;
}

void GlobalShortcutWin::remap() {
	HRESULT hr;

   	m_bNeedRemap = false;

	unacquire();
	for(int i=0;i<m_qlDevices.size();i++)
		m_qlDevices[i]->Release();
	m_qlDevices.clear();

	if (m_qmShortcuts.isEmpty())
		return;

	if (m_diaActions)
		delete [] m_diaActions;

	m_diaActions = new DIACTION[m_qmShortcuts.count()];

	int cnt = 0;
	QMapIterator<int, GlobalShortcut *> i(m_qmShortcuts);
	while (i.hasNext()) {
		i.next();
		GlobalShortcut *gs = i.value();
		memset(&m_diaActions[cnt], 0, sizeof(DIACTION));
		m_diaActions[cnt].uAppData = gs->idx;
		m_diaActions[cnt].dwSemantic = DIBUTTON_ANY(gs->idx);
		m_diaActions[cnt].lptszActionName = (const wchar_t *) gs->name.utf16();
	}

    ZeroMemory( &m_diafGame, sizeof(DIACTIONFORMAT) );
    m_diafGame.dwSize          = sizeof(DIACTIONFORMAT);
    m_diafGame.dwActionSize    = sizeof(DIACTION);
    m_diafGame.dwDataSize      = m_qmShortcuts.count() * sizeof(DWORD);
    m_diafGame.guidActionMap   = g_guidApp;
    m_diafGame.dwGenre         = DIVIRTUAL_REMOTE_CONTROL;
    m_diafGame.dwNumActions    = m_qmShortcuts.count();
    m_diafGame.rgoAction       = m_diaActions;
    m_diafGame.lAxisMin        = -100;
    m_diafGame.lAxisMax        = 100;
    m_diafGame.dwBufferSize    = DX_SAMPLE_BUFFER_SIZE;
    wcscpy( m_diafGame.tszActionMap, (const wchar_t *) QString("Mumble").utf16() );

	hr=m_pDI->EnumDevicesBySemantics((wchar_t *)qsUsername.utf16(), &m_diafGame, EnumSuitableDevicesCB, this, 0L);
	if (FAILED(hr))
		qWarning("GlobalShortcutWin: EnumDevicesBySemantics failed 0x%08x", hr);
}

BOOL GlobalShortcutWin::EnumSuitableDevicesCB(LPCDIDEVICEINSTANCE pdidi, LPDIRECTINPUTDEVICE8 pdidDevice, DWORD, DWORD, LPVOID pContext)
{
	GlobalShortcutWin *gsw=static_cast<GlobalShortcutWin *>(pContext);
	HRESULT hr;

	pdidDevice->Unacquire();

	pdidDevice->SetCooperativeLevel( g_mwMainWindow->winId(), DISCL_NONEXCLUSIVE|DISCL_BACKGROUND );

    hr = pdidDevice->BuildActionMap( &gsw->m_diafGame, (wchar_t *)gsw->qsUsername.utf16(), 0 );
    if( FAILED(hr) ) {
		qWarning("GlobalShortcutWin: Failed BuildActionMap: %d", hr);
	}

    // Set the action map for the current device
    hr = pdidDevice->SetActionMap( &gsw->m_diafGame, (wchar_t *)gsw->qsUsername.utf16(), 0 );
    if( FAILED(hr) ) {
		qWarning("GlobalShortcutWin: Failed SetActionMap: %d", hr);
	}

	pdidDevice->AddRef();

	gsw->m_qlDevices.append(pdidDevice);

    return DIENUM_CONTINUE;
}

void GlobalShortcutWin::configure()
{
    HRESULT hr;

    remap();

    // Initialize all the colors here
    DICOLORSET dics;
    ZeroMemory(&dics, sizeof(DICOLORSET));
    dics.dwSize = sizeof(DICOLORSET);

    // Fill in all the params
    DICONFIGUREDEVICESPARAMS dicdp;
    ZeroMemory(&dicdp, sizeof(dicdp));
    dicdp.dwSize = sizeof(dicdp);
    dicdp.dwcFormats     = 1;
    dicdp.lprgFormats    = &m_diafGame;
    dicdp.hwnd           = g_mwMainWindow->winId();
    dicdp.lpUnkDDSTarget = NULL;
    dicdp.dwcUsers       = 1;
    dicdp.lptszUserNames = (wchar_t *)qsUsername.utf16();

	unacquire();

    hr = m_pDI->ConfigureDevices( NULL, &dicdp, DICD_EDIT, NULL );
    if( FAILED(hr) ) {
		qWarning("GlobalShortcutWin: ConfigureDevices failed 0x%08x", hr);
		return;
	}

	remap();
}


GlobalShortcutWin::~GlobalShortcutWin() {
	unacquire();
	for(int i=0;i<m_qlDevices.size();i++)
		m_qlDevices[i]->Release();
	m_qlDevices.clear();

	m_pDI->Release();
}

void GlobalShortcutWin::unacquire() {
	for(int i=0;i<m_qlDevices.size();i++)
		m_qlDevices[i]->Unacquire();
}

void GlobalShortcutWin::timeTicked() {
    DIDEVICEOBJECTDATA didod[ DX_SAMPLE_BUFFER_SIZE ];
    DWORD dwElements;
    DWORD i;
    HRESULT hr;

    if (m_bNeedRemap)
    	remap();

	for(int i=0;i<m_qlDevices.size();i++) {
        DIDEVICEOBJECTDATA rgdod[DX_SAMPLE_BUFFER_SIZE];
        DWORD   dwItems = DX_SAMPLE_BUFFER_SIZE;
        HRESULT hr;

        m_qlDevices[i]->Acquire();
        m_qlDevices[i]->Poll();

        hr = m_qlDevices[i]->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), rgdod, &dwItems, 0 );
        if( FAILED(hr) )
            continue;

        for( DWORD j=0; j<dwItems; j++ )
        {
            bool  bButtonDown = (rgdod[j].dwData==0x80) ? true : false;
            if (m_qmShortcuts.contains(rgdod[j].uAppData)) {
				GlobalShortcut *gs = m_qmShortcuts[rgdod[j].uAppData];
				emit gs->triggered(bButtonDown);
				if (bButtonDown)
					emit gs->down();
				else
					emit gs->up();
			}
		}
	}
}

GlobalShortcut::GlobalShortcut(QObject *parent, int index, QString qsName) : QObject(parent) {
	if (! gsw)
	  gsw = new GlobalShortcutWin();
	gsw->ref++;
	idx = index;
	name=qsName;
	gsw->add(this);
}

GlobalShortcut::~GlobalShortcut()
{
	gsw->remove(this);
	gsw->ref--;
	if (gsw->ref == 0) {
		delete gsw;
		gsw = 0;
	}
}

void GlobalShortcut::configure()
{
	gsw->configure();
}
