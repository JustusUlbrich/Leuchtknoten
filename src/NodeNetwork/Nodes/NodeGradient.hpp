#pragma once
#include <string>

#include "../api/DataGradient.hpp"
#include "../api/INode.hpp"
#include "../api/Port.hpp"

#include "../../context.hpp"

namespace Node
{
	class NodeGradient : public INode
	{

	public:
		NodeGradient(/* args */);
		~NodeGradient();

		void eval(const Context &context, const LedContext &ledContext, DataRgb &out) override;
		void connectOutport(const std::string &portID, Connection<DataRgb> &connection) override;

		std::shared_ptr<InputPort<float>> scaleIn;
		std::shared_ptr<OutputPort<DataRgb>> out;
		DataGradient gradient;
	};

	NodeGradient::NodeGradient(/* args */)
	{
		scaleIn = std::make_shared<InputPort<float>>("scale", this);
		out = std::make_shared<OutputPort<DataRgb>>("num", this);
	}

	NodeGradient::~NodeGradient()
	{
	}

	void NodeGradient::eval(const Context &context, const LedContext &ledContext, DataRgb &out)
	{
		// float scale = 0.5;
		// DEBUG
		float scale = ledContext.id / 10.0f;

		if (this->scaleIn->connection.has_value())
		{
			auto con = this->scaleIn->connection.value();
			scale = con.fromPort->eval(context, ledContext);
		}

		if (gradient.entries.size() == 0)
			out = DataRgb();

		if (gradient.entries.size() == 1)
			out = gradient.entries[0].color;

		for (size_t i = 0; i < gradient.entries.size() - 1; i++)
		{
			if (gradient.entries[i + 1].offset < scale)
				continue;

			const auto cLeft = gradient.entries[i];
			const auto cRight = gradient.entries[i + 1];

			// TODO: clamp and verify
			const auto sOff = (scale - cLeft.offset) / (cRight.offset - cLeft.offset);

			out.r = (sOff * cLeft.color.r + (1.0 - sOff) * cRight.color.r);
			out.g = (sOff * cLeft.color.g + (1.0 - sOff) * cRight.color.g);
			out.b = (sOff * cLeft.color.b + (1.0 - sOff) * cRight.color.b);
			return;
		}

		out = gradient.entries[gradient.entries.size() - 1].color;
	}

	void NodeGradient::connectOutport(const std::string &portID, Connection<DataRgb> &connection)
	{
		connection.fromPort = std::shared_ptr<OutputPort<DataRgb>>(out);
	}

} // namespace Node