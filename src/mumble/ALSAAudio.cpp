/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "ALSAAudio.h"
#include "Player.h"
#include "Global.h"
#include <sys/poll.h>

#define NBLOCKS 8

#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

class ALSAEnumerator {
  public:
    QHash<QString,QString> qhInput;
    QHash<QString,QString> qhOutput;
    ALSAEnumerator();
};

static AudioOutput *ALSAAudioOutputNew()
{
    return new ALSAAudioOutput();
}

static AudioInput *ALSAAudioInputNew()
{
    return new ALSAAudioInput();
}

static AudioOutputRegistrar aorALSA(QLatin1String("ALSA"), ALSAAudioOutputNew);
static AudioInputRegistrar airALSA(QLatin1String("ALSA"), ALSAAudioInputNew);

static ConfigWidget *ALSAConfigDialogNew() {
        return new ALSAConfig();
}
        
static ConfigRegistrar registrar(20, ALSAConfigDialogNew);

static ALSAEnumerator cards;

ALSAEnumerator::ALSAEnumerator() {
  qhInput.insert(QLatin1String("default"), QLatin1String("Default ALSA Card"));
  qhOutput.insert(QLatin1String("default"), QLatin1String("Default ALSA Card"));

  int card=-1;
  snd_card_next(&card);
  while (card != -1) {
    char *name;
    snd_ctl_t *ctl=NULL;
    snd_card_get_longname(card, &name);
    QByteArray dev=QString::fromLatin1("hw:%1").arg(card).toUtf8();
    if (snd_ctl_open(&ctl, dev.data(), SND_CTL_READONLY) >= 0) {
      snd_pcm_info_t *info = NULL;
      snd_pcm_info_malloc(&info);

      char *cname = NULL;
      snd_card_get_name(card, &cname);

      int device = -1;
      snd_ctl_pcm_next_device(ctl, &device);
      
      bool play = false;
      bool cap = false;
      
      while (device != -1) {
        QString devname=QString::fromLatin1("hw:%1,%2").arg(card).arg(device);
        snd_pcm_info_set_device(info, device);
        snd_pcm_info_set_stream(info, SND_PCM_STREAM_CAPTURE);
        if (snd_ctl_pcm_info(ctl,info) == 0) {
          QString fname=QString::fromLatin1(snd_pcm_info_get_name(info));
          qhInput.insert(devname,fname);
          qhInput.insert(QString::fromLatin1("plug:\"")+devname+QString::fromLatin1("\""), fname);
          cap = true;
        }

        snd_pcm_info_set_stream(info, SND_PCM_STREAM_PLAYBACK);
        if (snd_ctl_pcm_info(ctl,info) == 0) {
          QString fname=QString::fromLatin1(snd_pcm_info_get_name(info));
          qhOutput.insert(devname,fname);
          qhOutput.insert(QString::fromLatin1("plug:")+devname, fname);
          play = true;
        }

        snd_ctl_pcm_next_device(ctl, &device);
      }
      if (play) {
        qhOutput.insert(QString::fromLatin1("plug:\"dmix:CARD=%1\"").arg(card),QLatin1String(cname));
      }
      if (cap) {
        qhInput.insert(QString::fromLatin1("plug:\"dsnoop:CARD=%1\"").arg(card),QLatin1String(cname));
      }
      snd_pcm_info_free(info);
      snd_ctl_close(ctl);
    }
    snd_card_next(&card);
  }
}

ALSAConfig::ALSAConfig(QWidget *p) : ConfigWidget(p) {
  QGroupBox *qgbDevices, *qgbOutput;
  QGridLayout *grid;
  QVBoxLayout *v;
  QLabel *l;
  
  qcbInputDevice = new QComboBox();
  qcbOutputDevice = new QComboBox();
  
  QList<QString> qlOutputDevs = cards.qhOutput.keys();
  qSort(qlOutputDevs);
  QList<QString> qlInputDevs = cards.qhInput.keys();
  qSort(qlInputDevs);

  bool found;
  
  
  found = false;
  foreach(QString dev, qlInputDevs) {
    QString t=QString::fromLatin1("[%1] %2").arg(dev).arg(cards.qhInput[dev]);
    qcbInputDevice->addItem(t, dev);
    if (dev == g.s.qsALSAInput) {
      found = true;
      qcbInputDevice->setCurrentIndex(qcbInputDevice->count() - 1);
    }
  }
  if (! found) {
    qcbInputDevice->addItem(g.s.qsALSAInput, g.s.qsALSAInput);
    qcbInputDevice->setCurrentIndex(qcbInputDevice->count() - 1);
  }

  found = false;
  foreach(QString dev, qlOutputDevs) {
    QString t=QString::fromLatin1("[%1] %2").arg(dev).arg(cards.qhOutput[dev]);
    qcbOutputDevice->addItem(t, dev);
    if (dev == g.s.qsALSAOutput) {
      found = true;
      qcbOutputDevice->setCurrentIndex(qcbOutputDevice->count() - 1);
    }
  }
  if (! found) {
    qcbOutputDevice->addItem(g.s.qsALSAOutput, g.s.qsALSAOutput);
    qcbOutputDevice->setCurrentIndex(qcbOutputDevice->count() - 1);
  }
  
  qgbDevices = new QGroupBox(tr("Device selection"));
  grid=new QGridLayout();
  
  qcbInputDevice->setToolTip(tr("Device to use for microphone"));
  qcbInputDevice->setWhatsThis(tr("This set which device mumble should use. The <i>default</i> device is whatever you have configured in alsaconfig, the <i>hwplug</i> "
                          "devices are specific hardware devices backed by the ALSA mixer and the <i>hw</i> devices are raw hardware access. Unless your soundcard "
                          "supports hardware mixing of audio, using the <i>hw</i> device will exclude all other programs from using audio."));
  qcbOutputDevice->setToolTip(tr("Device to use for speakers/headphones"));
  qcbOutputDevice->setWhatsThis(qcbInputDevice->whatsThis());

  l = new QLabel(tr("Input"));
  l->setBuddy(qcbInputDevice);
  grid->addWidget(l, 0, 0);
  grid->addWidget(qcbInputDevice, 0, 1);
  
  l = new QLabel(tr("Output"));
  l->setBuddy(qcbOutputDevice);
  grid->addWidget(l, 1, 0);
  grid->addWidget(qcbOutputDevice, 1, 1);
  
  qgbDevices->setLayout(grid);  

  qgbOutput = new QGroupBox(tr("Output Options"));
  grid = new QGridLayout();
  
  qsOutputDelay = new QSlider(Qt::Horizontal);
  qsOutputDelay->setRange(1, 6);
  
  qsOutputDelay->setSingleStep(1);
  qsOutputDelay->setPageStep(2);
  qsOutputDelay->setValue(g.s.iDXOutputDelay);
  qsOutputDelay->setObjectName(QLatin1String("OutputDelay"));
  l = new QLabel(tr("Output Delay"));
  l->setBuddy(qsOutputDelay);
  qlOutputDelay=new QLabel();
  qlOutputDelay->setMinimumWidth(30);
  on_OutputDelay_valueChanged(qsOutputDelay->value());
  qsOutputDelay->setToolTip(tr("Amount of data to buffer for ALSA"));
  qsOutputDelay->setWhatsThis(tr("This sets the amount of data to prebuffer in the output buffer. "
                                 "Experiment with different values and set it to the lowest which doesn't "
                                 "cause rapid jitter in the sound."));
  grid->addWidget(l, 0, 0);
  grid->addWidget(qsOutputDelay, 0, 1);
  grid->addWidget(qlOutputDelay, 0, 2);

  qgbOutput->setLayout(grid);
  
  v = new QVBoxLayout();
  v->addWidget(qgbDevices);
  v->addWidget(qgbOutput);
  v->addStretch(1);
  setLayout(v);
  QMetaObject::connectSlotsByName(this);
}

QString ALSAConfig::title() const {
  return tr("ALSA");
}

QIcon ALSAConfig::icon() const {
  return QIcon(QLatin1String(":/config_dsound.png"));
}

void ALSAConfig::accept() {
  g.s.iDXOutputDelay = qsOutputDelay->value();
  g.s.qsALSAInput = qcbInputDevice->itemData(qcbInputDevice->currentIndex()).toString();
  g.s.qsALSAOutput = qcbOutputDevice->itemData(qcbOutputDevice->currentIndex()).toString();
}

void ALSAConfig::on_OutputDelay_valueChanged(int v) {
  qlOutputDelay->setText(tr("%1ms").arg(v*20));
}
        
ALSAAudioInput::ALSAAudioInput()
{
}

ALSAAudioInput::~ALSAAudioInput()
{
    // Signal input thread to end
    bRunning = false;
    wait();
}

void ALSAAudioInput::run()
{
    int readblapp;


    QByteArray device_name = g.s.qsALSAInput.toLatin1();
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_t *capture_handle;
    
//    device_name = "default";

    unsigned int rrate = SAMPLE_RATE;

    bRunning = true;
    int err = 0;

    qWarning("ALSAAudioInput: Initing audiocapture %s.",device_name.data());
    err = snd_pcm_open(&capture_handle, device_name.data(), SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) qWarning("ALSAAudioInput: %s", snd_strerror(err));

    /* Setup parameters using hw_params structure */
    err = snd_pcm_hw_params_malloc(&hw_params);
    if (err < 0) qWarning("ALSAAudioInput: Allocating Memory: %s", snd_strerror(err));

    err = snd_pcm_hw_params_any(capture_handle, hw_params);
    if (err < 0) qWarning("ALSAAudioInput: Any Settings: %s", snd_strerror(err));

    err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) qWarning("ALSAAudioInput: Access: %s", snd_strerror(err));

    err = snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    if (err < 0) qWarning("ALSAAudioInput: Format: %s", snd_strerror(err));

    err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rrate, NULL);
    if (err < 0) qWarning("ALSAAudioInput: Rate: %s", snd_strerror(err));

    err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, 1);
    if (err < 0) qWarning("ALSAAudioInput: Channels: %s", snd_strerror(err));

    snd_pcm_uframes_t wantPeriod = iFrameSize;
    err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &wantPeriod, NULL);
    if (err < 0) qWarning("ALSAAudioInput: Period Size: %s", snd_strerror(err));

    snd_pcm_uframes_t wantBuff = wantPeriod * 4;
    err = snd_pcm_hw_params_set_buffer_size_near(capture_handle, hw_params, &wantBuff);
    if (err < 0) qWarning("ALSAAudioInput: Buffer Size: %s", snd_strerror(err));

    err = snd_pcm_hw_params(capture_handle, hw_params);
    if (err < 0) qWarning("ALSAAudioInput: hw params: %s", snd_strerror(err));
    
    qWarning("ALSAAudioInput: Actual buffer %ld samples [%ld(%d) per period]",wantBuff,wantPeriod,iFrameSize);

    snd_pcm_hw_params_free(hw_params);

/*
 * Add this information to either very very very vers verbose output
 * or debug output
 */
    snd_output_t *log;
    // snd_pcm_status_t *status;
    snd_output_stdio_attach(&log, stderr,0 );
    snd_pcm_dump(capture_handle, log);

    /* Prepare device */
    err = snd_pcm_prepare(capture_handle);
    if (err < 0) qWarning("ALSAAudioInput: Prepare: %s", snd_strerror(err));

    /* Start device - not necessary */
    err = snd_pcm_start(capture_handle);
    if (err < 0) qWarning("ALSAAudioInput: Start: %s", snd_strerror(err));

    while (bRunning) {
/*
 * Add this information to either very very very very verbose output
 * or debug output.
	snd_pcm_status_malloc(&status);
	snd_pcm_status(capture_handle, status);
	snd_pcm_status_dump(status, log);
	snd_pcm_status_free(status);
 */
	readblapp = snd_pcm_readi(capture_handle, psMic, iFrameSize);
	if (readblapp == -ESTRPIPE) {
	    // suspend event - what to do?
	    qWarning("ALSAAudioInput: %s", snd_strerror(readblapp));
	} else if (readblapp == -EPIPE) {
	    err = snd_pcm_prepare(capture_handle);
	    qWarning("ALSAAudioInput: %s: %s", snd_strerror(readblapp), snd_strerror(err));
	} else if (readblapp < 0) {
	    err = snd_pcm_prepare(capture_handle);
	    qWarning("ALSAAudioInput: %s: %s", snd_strerror(readblapp), snd_strerror(err));
	} else if (iFrameSize == readblapp) {
	    encodeAudioFrame();
	}
    }

    snd_pcm_drain(capture_handle);
    snd_pcm_close(capture_handle);

    qWarning("ALSAAudioInput: Releasing ALSA Mic.");
}

ALSAOutputPlayer::ALSAOutputPlayer(ALSAAudioOutput * ao, Player * player):AudioOutputPlayer(ao, player)
{
    aao = static_cast < ALSAAudioOutput * >(aoOutput);
    pcm_handle = NULL;
    iAliveHold = 0;
}

ALSAOutputPlayer::~ALSAOutputPlayer()
{
    if (pcm_handle) {
		snd_pcm_close(pcm_handle);
    }
    // If playing, stop
    qWarning("ALSAOutputPlayer: %s: Removed", qPrintable(p->qsName));
}

bool ALSAOutputPlayer::playFrames()
{
    int avail;
    bool alive;

    avail = snd_pcm_avail_update(pcm_handle);
    if (avail < 0)
	return false;
    while (avail >= iFrameSize) {
		alive = decodeNextFrame();
		snd_pcm_writei(pcm_handle, psBuffer, iFrameSize);
		avail = snd_pcm_avail_update(pcm_handle);
		if (avail < 0)
			return false;
		if (alive)
			iAliveHold = 0;
		else {
			iAliveHold++;
			if (iAliveHold > 50) {
			return false;
			}
		}
    }
    return true;
}

void ALSAOutputPlayer::initialize()
{
    if (pcm_handle)
		return;

    unsigned int rate = SAMPLE_RATE;
    snd_pcm_uframes_t period_size = iFrameSize;
    snd_pcm_uframes_t buffer_size = iFrameSize * 5;

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    /* The device name */
    QByteArray device_name = g.s.qsALSAOutput.toLatin1();

    /* Open the device */
    snd_pcm_open(&pcm_handle, device_name.data(), SND_PCM_STREAM_PLAYBACK, 0);

    /* Error check */

    snd_pcm_hw_params_malloc(&hw_params);
    snd_pcm_hw_params_any(pcm_handle, hw_params);

    snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rate, NULL);
    snd_pcm_hw_params_set_channels(pcm_handle, hw_params, 1);

    snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hw_params, &buffer_size);
    snd_pcm_hw_params_set_period_size_near(pcm_handle, hw_params, &period_size, NULL);

    /* Apply parameters */
    snd_pcm_hw_params(pcm_handle, hw_params);

    /* Free HW structure */
    snd_pcm_hw_params_free(hw_params);

    /* Software paramters */
    snd_pcm_sw_params_malloc(&sw_params);
    snd_pcm_sw_params_current(pcm_handle, sw_params);

    snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, 0);
    snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size);

    snd_pcm_sw_params(pcm_handle, sw_params);
    snd_pcm_sw_params_free(sw_params);

    /* Prepare device */
    snd_pcm_prepare(pcm_handle);

    // Fill one frame
    for (int i = 0; i < 5; i++)
		snd_pcm_writei(pcm_handle, psBuffer, iFrameSize);

    snd_pcm_start(pcm_handle);
}

ALSAAudioOutput::ALSAAudioOutput()
{
    qWarning("ALSAAudioOutput: Initialized");
}

ALSAAudioOutput::~ALSAAudioOutput()
{
    bRunning = false;
    // Call destructor of all children
    wipe();
    // Wait for terminate
    wait();
    qWarning("ALSAAudioOutput: Destroyed");
}

AudioOutputPlayer *ALSAAudioOutput::getPlayer(Player * player)
{
    return new ALSAOutputPlayer(this, player);
}

#define MAX_FDS 256

void ALSAAudioOutput::run()
{
    struct pollfd fds[MAX_FDS];
    int count, idx;
    QSet <ALSAOutputPlayer *> del;
    QMap <ALSAOutputPlayer *, int> map;
    QMap <ALSAOutputPlayer *, int> cmap;


    bRunning = true;

    ALSAOutputPlayer *aaop;
    AudioOutputPlayer *aop;

    while (bRunning) {
		del.clear();
		map.clear();
		cmap.clear();

		qrwlOutputs.lockForRead();
		idx = 0;
		foreach(aop, qmOutputs) {
			aaop = static_cast < ALSAOutputPlayer * >(aop);
			aaop->initialize();
			count = snd_pcm_poll_descriptors_count(aaop->pcm_handle);
			snd_pcm_poll_descriptors(aaop->pcm_handle, &fds[idx], count);
			map[aaop] = idx;
			cmap[aaop] = count;
			idx += count;
		}
		qrwlOutputs.unlock();

		poll(fds, idx, 20);

		qrwlOutputs.lockForRead();
		foreach(aop, qmOutputs) {
			unsigned short revents;
			aaop = static_cast < ALSAOutputPlayer * >(aop);
			idx = map[aaop];
			count = cmap[aaop];
			if (count) {
				snd_pcm_poll_descriptors_revents(aaop->pcm_handle, &fds[idx], count, &revents);
				if (revents & POLLERR) {
					del.insert(aaop);
				} else if (revents & POLLOUT) {
					if (!aaop->playFrames())
					del.insert(aaop);
				}
			}
		}
		qrwlOutputs.unlock();

		foreach(aaop, del) {
			removeBuffer(aaop->p);
		}
    }
}
