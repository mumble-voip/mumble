#pragma once

#include <clunk/hrtf.h>

class Clunk
{
public:


	void process(SampleBuffer& samples, Direction3D pos, AngleRadians direction)
	{
		auto [x, y, z] = pos.get();

		//| cos θ - sin θ   0 | | x | | x cos θ - y sin θ | | x'|
		//| sin θ    cos θ   0 | | y | = | x sin θ + y cos θ | = | y'|
		//| 0       0      1 | | z | | z | | z'|


		const auto x_ = x * direction.cosine() - y * direction.sine();
		const auto y_ = x * direction.sine() + y * direction.cosine();	

        auto channels = samples.getChannels();
        auto sampleCount = samples.getSampleCount();
		input_buffer.insert(input_buffer.end(), samples.begin(), samples.end());

		if (input_buffer.size() > clunk::Hrtf::WINDOW_SIZE * channels * 2u)
		{

			const auto to_process = static_cast<int>(input_buffer.size());

			clunk::Buffer src(to_process * sizeof(short));
			src.set_data(input_buffer.data(), to_process * sizeof(short), false);//This is used as const so we can just pass input_buffers data ptr


			const auto output_size = to_process - clunk::Hrtf::WINDOW_SIZE * channels * 2;

			short* dst_s = new short[output_size]; //Will be owned by clunk::Buffer	which takes care of deleting
			memset(dst_s, 0, output_size * sizeof(short));
			clunk::Buffer dst(output_size * sizeof(short));
			dst.set_data(dst_s, output_size * sizeof(short), true);

			const auto processed = hrft.process(SAMPLE_RATE, dst, channels, src, channels, clunk::v3f(x_, y_, z), 1.0f);
			input_buffer.erase(input_buffer.begin(), input_buffer.begin() + processed * channels); //Erase processed data and keep leftover
			
			output_buffer.insert(output_buffer.end(), dst_s, dst_s + output_size);

		}

		if (output_buffer.size() < sampleCount * channels) {
            samples.setToNull();
		} else {
			memcpy(samples.begin(), output_buffer.data(), sampleCount * channels * sizeof(short));
			output_buffer.erase(output_buffer.begin(), output_buffer.begin() + sampleCount * channels);
		}

	}

	~Clunk()
	{
		input_buffer.clear();
		output_buffer.clear();		
	}
	
	
private:
	clunk::Hrtf hrft;
	std::vector<short> input_buffer;
	std::vector<short> output_buffer;	
};
