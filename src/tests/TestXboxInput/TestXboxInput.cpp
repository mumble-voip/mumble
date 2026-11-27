// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListenerManager.h"
#include "XboxInput.h"
#include "Global.h"
#include "GlobalShortcut_win.h"

#include <QSignalSpy>
#include <QTemporaryDir>
#include <QtTest>

#include <array>
#include <limits>
#include <memory>
#include <mutex>
#include <utility>

extern HWND mumble_mw_hwnd;

namespace {
struct Device {
	uint32_t result      = ERROR_DEVICE_NOT_CONNECTED;
	XboxInputState state = {};
	uint32_t queries     = 0;
};

std::array< Device, XBOXINPUT_MAX_DEVICES > s_devices;
std::mutex s_deviceMutex;

uint32_t WINAPI getState(uint32_t index, XboxInputState *state) {
	std::lock_guard< std::mutex > lock(s_deviceMutex);
	Device &device = s_devices.at(index);
	++device.queries;
	if (device.result == ERROR_SUCCESS) {
		*state = device.state;
	}
	return device.result;
}

Device device(uint32_t index) {
	std::lock_guard< std::mutex > lock(s_deviceMutex);
	return s_devices.at(index);
}

void setDevice(uint32_t index, uint32_t result, uint32_t packet, uint16_t buttons) {
	std::lock_guard< std::mutex > lock(s_deviceMutex);
	Device &entry            = s_devices.at(index);
	entry.result             = result;
	entry.state.packetNumber = packet;
	entry.state.buttons      = buttons;
	entry.state.leftTrigger  = buttons ? 255 : 0;
}

std::unique_ptr< XboxInput > createInput() {
	auto input      = std::make_unique< XboxInput >();
	input->GetState = getState;
	return input;
}
} // namespace

class TestXboxInput : public QObject {
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();
	void init();
	void reconnect_data();
	void reconnect();
	void independentSlots();
	void invalidSlot();
	void workerPollsWithoutRawInput();
	void disabledWorkerStops();

private:
	QTemporaryDir m_directory;
	std::unique_ptr< Global > m_global;
};

void TestXboxInput::initTestCase() {
	QVERIFY(m_directory.isValid());
	m_global                = std::make_unique< Global >(m_directory.filePath("settings.json"));
	Global::g_global_struct = m_global.get();
	mumble_mw_hwnd          = CreateWindowExW(0, L"STATIC", L"TestXboxInput", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr,
                                     GetModuleHandleW(nullptr), nullptr);
	QVERIFY(mumble_mw_hwnd != nullptr);
}

void TestXboxInput::cleanupTestCase() {
	if (mumble_mw_hwnd) {
		DestroyWindow(mumble_mw_hwnd);
		mumble_mw_hwnd = nullptr;
	}
	Global::g_global_struct = nullptr;
	m_global.reset();
}

void TestXboxInput::init() {
	std::lock_guard< std::mutex > lock(s_deviceMutex);
	s_devices                        = {};
	Global::get().s.bEnableXboxInput = true;
	Global::get().s.bEnableGKey      = false;
}

void TestXboxInput::reconnect_data() {
	QTest::addColumn< quint32 >("packet");
	QTest::addColumn< quint32 >("error");
	QTest::newRow("zero") << quint32(0) << quint32(ERROR_DEVICE_NOT_CONNECTED);
	QTest::newRow("nonzero") << quint32(42) << quint32(ERROR_DEVICE_NOT_CONNECTED);
	QTest::newRow("maximum") << std::numeric_limits< quint32 >::max() << quint32(ERROR_DEVICE_NOT_CONNECTED);
	QTest::newRow("query-error") << quint32(42) << quint32(ERROR_GEN_FAILURE);
}

void TestXboxInput::reconnect() {
	QFETCH(quint32, packet);
	QFETCH(quint32, error);
	auto input           = createInput();
	XboxInputState state = {};
	QVERIFY(!input->poll(0, state));

	setDevice(0, ERROR_SUCCESS, packet, 0x1000);
	QVERIFY(input->poll(0, state));
	QCOMPARE(state.buttons, uint16_t(0x1000));
	QCOMPARE(state.leftTrigger, uint8_t(255));
	QVERIFY(!input->poll(0, state));
	QCOMPARE(device(0).queries, uint32_t(3));

	setDevice(0, error, packet, 0x1000);
	QVERIFY(input->poll(0, state));
	QCOMPARE(state.buttons, uint16_t(0));
	QCOMPARE(state.leftTrigger, uint8_t(0));
	QVERIFY(!input->poll(0, state));

	setDevice(0, ERROR_SUCCESS, packet, 0x2000);
	QVERIFY(input->poll(0, state));
	QCOMPARE(state.buttons, uint16_t(0x2000));
	QVERIFY(!input->poll(0, state));
}

void TestXboxInput::independentSlots() {
	auto input           = createInput();
	XboxInputState state = {};
	setDevice(0, ERROR_SUCCESS, 7, 0x1000);
	setDevice(1, ERROR_SUCCESS, 7, 0x2000);
	QVERIFY(input->poll(0, state));
	QVERIFY(input->poll(1, state));
	setDevice(0, ERROR_DEVICE_NOT_CONNECTED, 7, 0x1000);
	QVERIFY(input->poll(0, state));
	QVERIFY(!input->poll(1, state));
	setDevice(1, ERROR_SUCCESS, 8, 0x4000);
	QVERIFY(input->poll(1, state));
	QCOMPARE(state.buttons, uint16_t(0x4000));
}

void TestXboxInput::invalidSlot() {
	auto input           = createInput();
	XboxInputState state = {};
	QVERIFY(!input->poll(XBOXINPUT_MAX_DEVICES, state));
	QCOMPARE(device(0).queries, uint32_t(0));
	input->GetState = nullptr;
	QVERIFY(!input->poll(0, state));
}

void TestXboxInput::workerPollsWithoutRawInput() {
	auto input = createInput();
	QVERIFY(input->isValid());
	GlobalShortcutWin worker(std::move(input));
	QSignalSpy buttons(&worker, &GlobalShortcutEngine::buttonPressed);

	// No raw input devices or messages are supplied to the worker.
	QTRY_VERIFY_WITH_TIMEOUT(device(0).queries >= 3, 1000);
	QTest::qWait(120);
	setDevice(0, ERROR_SUCCESS, 42, 0x1000);
	QTRY_COMPARE_WITH_TIMEOUT(buttons.count(), 2, 1000);
	QVERIFY(!buttons.at(0).at(0).toBool());
	QVERIFY(!buttons.at(1).at(0).toBool());

	setDevice(0, ERROR_DEVICE_NOT_CONNECTED, 42, 0x1000);
	worker.deviceRemoved(reinterpret_cast< HANDLE >(1));
	QTRY_COMPARE_WITH_TIMEOUT(buttons.count(), 4, 1000);
	QVERIFY(buttons.at(2).at(0).toBool());
	QVERIFY(buttons.at(3).at(0).toBool());
	const uint32_t queries = device(0).queries;
	QTRY_VERIFY_WITH_TIMEOUT(device(0).queries > queries, 1000);

	setDevice(0, ERROR_SUCCESS, 42, 0x1000);
	QTRY_COMPARE_WITH_TIMEOUT(buttons.count(), 6, 1000);
	QVERIFY(!buttons.at(4).at(0).toBool());
	QVERIFY(!buttons.at(5).at(0).toBool());
}

void TestXboxInput::disabledWorkerStops() {
	Global::get().s.bEnableXboxInput = false;
	GlobalShortcutWin worker(createInput());
	QTest::qWait(50);
	QCOMPARE(device(0).queries, uint32_t(0));
	worker.requestInterruption();
	worker.deviceRemoved(reinterpret_cast< HANDLE >(1));
	QVERIFY(worker.wait(1000));
}

QTEST_MAIN(TestXboxInput)
#include "TestXboxInput.moc"
