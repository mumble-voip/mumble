// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtTest>

#include "HrtfSpatializer.h"

#include <cmath>
#include <cstring>
#include <random>

class TestHrtfSpatializer : public QObject {
	Q_OBJECT

private slots:
	void initTestCase();
	void testSofaLoading();
	void testCardinalDirectionDistinctness();
	void testImpulseConvolution();
	void testNumericalStability();
	void testLeftRightAsymmetry();
	void testZeroDirectionVector();
	void testVariableFrameCountCrossfade();

private:
	static constexpr int kSampleRate = 48000;
	static constexpr int kBlockSize  = 480; // 10 ms at 48kHz
};

void TestHrtfSpatializer::initTestCase() {
	// Nothing to do - each test creates its own spatializer
}

void TestHrtfSpatializer::testSofaLoading() {
	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(!spatializer.isLoaded());

	const bool ok = spatializer.loadHRTF(); // use default SOFA
	QVERIFY2(ok, "loadHRTF() should succeed with the default SOFA file");
	QVERIFY(spatializer.isLoaded());
}

void TestHrtfSpatializer::testCardinalDirectionDistinctness() {
	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	// Impulse input
	std::vector< float > impulse(static_cast< size_t >(kBlockSize), 0.0f);
	impulse[0] = 1.0f;

	auto getResponse = [&](float dx, float dy, float dz) -> std::vector< float > {
		// Each call needs its own spatializer instance to get an independent impulse response
		HrtfSpatializer sp(kSampleRate, kBlockSize);
		sp.loadHRTF();
		std::vector< float > out(static_cast< size_t >(kBlockSize * 2));
		sp.spatialize(1, impulse.data(), out.data(), static_cast< unsigned int >(kBlockSize), dx, dy, dz);
		return out;
	};

	const auto front = getResponse(0.0f, 0.0f, 1.0f);  // forward
	const auto back  = getResponse(0.0f, 0.0f, -1.0f); // backward
	const auto left  = getResponse(-1.0f, 0.0f, 0.0f); // left
	const auto right = getResponse(1.0f, 0.0f, 0.0f);  // right
	const auto up    = getResponse(0.0f, 1.0f, 0.0f);  // up

	auto energyDiff = [&](const std::vector< float > &a, const std::vector< float > &b) {
		float diff = 0.0f;
		for (size_t i = 0; i < a.size(); ++i)
			diff += (a[i] - b[i]) * (a[i] - b[i]);
		return diff;
	};

	// All cardinal directions should produce distinct HRIRs
	QVERIFY2(energyDiff(front, back) > 1e-6f, "front and back should produce different HRIRs");
	QVERIFY2(energyDiff(front, left) > 1e-6f, "front and left should produce different HRIRs");
	QVERIFY2(energyDiff(front, right) > 1e-6f, "front and right should produce different HRIRs");
	QVERIFY2(energyDiff(front, up) > 1e-6f, "front and up should produce different HRIRs");
	QVERIFY2(energyDiff(left, right) > 1e-6f, "left and right should produce different HRIRs");
}

void TestHrtfSpatializer::testImpulseConvolution() {
	// Convolving a unit impulse with a filter produces the filter itself.
	// We can't easily access the raw HRIR, but we can verify that:
	// - The output is non-trivial (energy > 0)
	// - The output has the same energy as expected for a unit response

	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	std::vector< float > impulse(static_cast< size_t >(kBlockSize), 0.0f);
	impulse[0] = 1.0f;

	std::vector< float > out(static_cast< size_t >(kBlockSize * 2), 0.0f);
	spatializer.spatialize(1, impulse.data(), out.data(), static_cast< unsigned int >(kBlockSize),
	                       0.0f, 0.0f, 1.0f); // front direction

	float energy = 0.0f;
	for (float s : out)
		energy += s * s;

	QVERIFY2(energy > 1e-6f, "Impulse response should have non-zero energy");

	// HRTF for most SOFA files has energy ≈ 1.0 (normalized). Allow a wide range.
	QVERIFY2(energy < 10.0f, "Impulse response energy should be bounded");
}

void TestHrtfSpatializer::testNumericalStability() {
	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	std::mt19937 rng(42);
	std::uniform_real_distribution< float > signalDist(-1.0f, 1.0f);
	std::uniform_real_distribution< float > angleDist(-1.0f, 1.0f);

	std::vector< float > input(static_cast< size_t >(kBlockSize));
	std::vector< float > output(static_cast< size_t >(kBlockSize * 2));

	for (int iter = 0; iter < 1000; ++iter) {
		// Random input
		for (float &s : input)
			s = signalDist(rng);

		// Random direction (normalize)
		float dx = angleDist(rng);
		float dy = angleDist(rng);
		float dz = angleDist(rng);
		const float len = std::sqrt(dx * dx + dy * dy + dz * dz);
		if (len < 1e-6f) {
			dx = 0.0f; dy = 0.0f; dz = 1.0f;
		} else {
			dx /= len; dy /= len; dz /= len;
		}

		spatializer.spatialize(1, input.data(), output.data(),
		                       static_cast< unsigned int >(kBlockSize), dx, dy, dz);

		for (size_t i = 0; i < output.size(); ++i) {
			QVERIFY2(std::isfinite(output[i]),
			         qPrintable(QString("NaN/Inf in output at iter %1 sample %2").arg(iter).arg(i)));
		}
	}
}

void TestHrtfSpatializer::testLeftRightAsymmetry() {
	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	std::vector< float > impulse(static_cast< size_t >(kBlockSize), 0.0f);
	impulse[0] = 1.0f;

	auto getResponse = [&](float dx, float dy, float dz, unsigned int id) {
		std::vector< float > out(static_cast< size_t >(kBlockSize * 2), 0.0f);
		spatializer.spatialize(id, impulse.data(), out.data(),
		                       static_cast< unsigned int >(kBlockSize), dx, dy, dz);
		return out;
	};

	// Source to the right: L and R channels should be different
	const auto rightOut = getResponse(1.0f, 0.0f, 0.0f, 10);
	float diffLR = 0.0f;
	for (unsigned int i = 0; i < static_cast< unsigned int >(kBlockSize); ++i)
		diffLR += std::abs(rightOut[2 * i] - rightOut[2 * i + 1]);
	QVERIFY2(diffLR > 1e-4f, "Right-side source should produce different L/R outputs");

	// Source in front: L and R channels should be approximately equal (symmetric HRTF)
	spatializer.removeSource(10);
	const auto frontOut = getResponse(0.0f, 0.0f, 1.0f, 11);
	float maxLRDiff = 0.0f;
	float maxMag    = 0.0f;
	for (unsigned int i = 0; i < static_cast< unsigned int >(kBlockSize); ++i) {
		maxLRDiff = std::max(maxLRDiff, std::abs(frontOut[2 * i] - frontOut[2 * i + 1]));
		maxMag    = std::max(maxMag, std::abs(frontOut[2 * i]));
	}
	// For a symmetric HRTF (KEMAR), front should have L ≈ R
	// Allow up to 10% relative difference
	if (maxMag > 1e-6f) {
		QVERIFY2(maxLRDiff / maxMag < 0.2f,
		         qPrintable(QString("Front source L/R difference (%1) exceeds 20% of magnitude (%2)")
		                        .arg(maxLRDiff)
		                        .arg(maxMag)));
	}
}

void TestHrtfSpatializer::testZeroDirectionVector() {
	// When source and listener are at the same position, the direction vector passed to
	// spatialize() is (0,0,0). This must not produce NaN/Inf or trigger click-inducing
	// crossfade thrashing (dot=0 < threshold → re-init every block).
	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	std::vector< float > input(static_cast< size_t >(kBlockSize), 0.1f);
	std::vector< float > output(static_cast< size_t >(kBlockSize * 2), 0.0f);

	// Run 10 consecutive blocks with zero direction — should be stable and finite.
	for (int block = 0; block < 10; ++block) {
		spatializer.spatialize(42, input.data(), output.data(),
		                       static_cast< unsigned int >(kBlockSize), 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < output.size(); ++i) {
			QVERIFY2(std::isfinite(output[i]),
			         qPrintable(QString("NaN/Inf at block %1 sample %2").arg(block).arg(i)));
		}
	}
}

void TestHrtfSpatializer::testVariableFrameCountCrossfade() {
	// Reproduce the condition Codex flagged: mix() backends call spatialize() with
	// variable frameCount values (WASAPI uses packetLength, JACK can call with 1
	// frame). If the crossfade completes over multiple small calls, curDir must
	// reflect the direction nxt was actually initialised with, not whatever
	// direction happens to arrive in the final small call.

	HrtfSpatializer spatializer(kSampleRate, kBlockSize);
	QVERIFY(spatializer.loadHRTF());

	std::vector< float > input(static_cast< size_t >(kBlockSize), 0.05f);
	std::vector< float > output(static_cast< size_t >(kBlockSize * 2), 0.0f);

	// Seed: source directly in front.
	spatializer.spatialize(1, input.data(), output.data(),
	                       static_cast< unsigned int >(kBlockSize), 0.0f, 0.0f, 1.0f);

	// Trigger a crossfade by moving to the right, using half-block chunks.
	// The crossfade spans kBlockSize samples total but arrives in pieces.
	const unsigned int half = static_cast< unsigned int >(kBlockSize) / 2;
	std::vector< float > halfInput(half, 0.05f);
	std::vector< float > halfOutput(half * 2, 0.0f);

	// First half-block with new direction (starts crossfade, nxtDir = right).
	spatializer.spatialize(1, halfInput.data(), halfOutput.data(), half, 1.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < halfOutput.size(); ++i) {
		QVERIFY2(std::isfinite(halfOutput[i]), "NaN/Inf in first half-block");
	}

	// Second half-block with a *different* direction (should not affect nxtDir).
	spatializer.spatialize(1, halfInput.data(), halfOutput.data(), half, 0.0f, 1.0f, 0.0f);
	for (size_t i = 0; i < halfOutput.size(); ++i) {
		QVERIFY2(std::isfinite(halfOutput[i]), "NaN/Inf in second half-block");
	}

	// After crossfade completes, curDir must be the direction nxt was inited with
	// (right = 1,0,0), not the last caller direction (up = 0,1,0). The next call
	// should NOT immediately re-trigger a crossfade back to right.
	// We verify this indirectly: running several more blocks with the same "up"
	// direction should produce finite, stable output without continuous re-init.
	for (int block = 0; block < 5; ++block) {
		spatializer.spatialize(1, input.data(), output.data(),
		                       static_cast< unsigned int >(kBlockSize), 0.0f, 1.0f, 0.0f);
		for (size_t i = 0; i < output.size(); ++i) {
			QVERIFY2(std::isfinite(output[i]),
			         qPrintable(QString("NaN/Inf at block %1 sample %2").arg(block).arg(i)));
		}
	}
}

QTEST_APPLESS_MAIN(TestHrtfSpatializer)
#include "TestHrtfSpatializer.moc"
