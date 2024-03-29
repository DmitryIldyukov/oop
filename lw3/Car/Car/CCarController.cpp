#include "CCar.h"
#include "CCarController.h"

std::string const INSTRUCTION = "----- ���������� -----\nInfo - �������� ���������� �� ����������\nEngineOn - �������� ���������\nEngineOff - ��������� ���������\nSetGear <��������> - ����������� ��������SetSpeed <��������> - �������� �������� ����������\nQuit - ����� �� ����������\n";
std::string const ENGINE_ON = "���������: �������";
std::string const ENGINE_OFF = "���������: ��������";
std::string const GET_GEAR = "��������: ";
std::string const GET_SPEED = "��������: ";
std::string const DIRECTION_STOP = "�����������: �� �����";
std::string const DIRECTION_FORWARD = "�����������: ������";
std::string const DIRECTION_BACKWARD = "�����������: �����";
std::string const ERROR_ENGINE_ON = "���������� ��������� ���������";
std::string const CORRECT_ENGINE_ON = "��������� �������";
std::string const ERROR_ENGINE_OFF = "���������� ��������� ���������";
std::string const CORRECT_ENGINE_OFF = "��������� ��������";
std::string const INCORRECT_USING_SET_GEAR = "������������ �������������: SetGear <gear>";
std::string const INVALID_ARGUMENT_GEAR = "�������� ������ ���� ����� ������";
std::string const ERROR_ARGUMENT_GEAR = "����� �������� �� ����������";
std::string const SET_GEAR = "�������� ��������: ";
std::string const INCORRECT_USING_SET_SPEED = "������������ �������������: SetSpeed <speed>";
std::string const INVALID_ARGUMENT_SPEED = "�������� ������ ���� ����� ������";
std::string const ERROR_ARGUMENT_SPEED = "����������� �������� �� ������ ��������";
std::string const SET_SPEED = "�������� �������� �� ";
std::string const UNKNOWN_COMMAND = "����������� �������! ���������� \"Help\" ����� �������� ���������� �� �������������";

CCarController::CCarController(CCar& car, std::istream& input, std::ostream& output)
	: m_car(car)
	, m_input(input)
	, m_output(output)
	, m_actionMap({
		{ "Help", bind(&CCarController::Help, this, std::placeholders::_1) },
		{ "Info", bind(&CCarController::Info, this, std::placeholders::_1) },
		{ "EngineOn", bind(&CCarController::EngineOn, this, std::placeholders::_1) },
		{ "EngineOff", bind(&CCarController::EngineOff, this, std::placeholders::_1) },
		{ "SetGear", bind(&CCarController::SetGear, this, std::placeholders::_1) },
		{ "SetSpeed", bind(&CCarController::SetSpeed, this, std::placeholders::_1) },
		{ "Quit", bind(&CCarController::Quit, this, std::placeholders::_1) },
		{ "Exit", bind(&CCarController::Quit, this, std::placeholders::_1) }
		})
{}

void CCarController::StartController()
{
	while (!m_input.eof() && !m_output.fail())
	{
		m_output << "> ";
		if (!CCarController::HandleCommand())
			m_output << UNKNOWN_COMMAND << std::endl;
	}
};

bool CCarController::HandleCommand()
{
	std::string comandLine;
	std::getline(m_input, comandLine);
	std::istringstream strm(comandLine);

	std::string action;
	strm >> action;

	auto it = m_actionMap.find(action);
	if (it != m_actionMap.end())
		return it->second(strm);

	return false;
}

bool CCarController::Help(std::istream& args)
{
	m_output << INSTRUCTION;
	return true;
}

bool CCarController::Info(std::istream& args)
{
	if (m_car.IsTurnedOn())
		m_output << ENGINE_ON << std::endl;
	else
		m_output << ENGINE_OFF << std::endl;
	m_output << GET_GEAR << m_car.GetGear() << std::endl;
	m_output << GET_SPEED << m_car.GetSpeed() << std::endl;
	switch (m_car.GetDirection())
	{
	case Direction::Forward:
		m_output << DIRECTION_FORWARD << std::endl;
		break;
	case Direction::Backward:
		m_output << DIRECTION_BACKWARD << std::endl;
		break;
	case Direction::Stop:
		m_output << DIRECTION_STOP << std::endl;
		break;
	}

	return true;
}

bool CCarController::EngineOn(std::istream& args)
{
	if (!m_car.TurnOnEngine()) {
		m_output << ERROR_ENGINE_ON << std::endl;
		return true;
	}
	m_output << CORRECT_ENGINE_ON << std::endl;
	return true;
}

bool CCarController::EngineOff(std::istream& args)
{
	if (!m_car.TurnOffEngine()) {
		m_output << ERROR_ENGINE_OFF << std::endl;
		return true;
	}
	m_output << CORRECT_ENGINE_OFF << std::endl;
	return true;
}

bool CCarController::SetGear(std::istream& args)
{
	if (args.eof())
	{
		m_output << INCORRECT_USING_SET_GEAR << std::endl;
		return true;
	}
	std::string gearStr;
	args >> gearStr;
	int gear;
	try
	{
		gear = std::stoi(gearStr);
	}
	catch (std::invalid_argument e)
	{
		m_output << INVALID_ARGUMENT_GEAR << std::endl;
		return true;
	}

	if (gear < -1 || gear > 5)
	{
		m_output << ERROR_ARGUMENT_GEAR << std::endl;
		return true;
	}

	if (m_car.SetGear(gear))
		m_output << SET_GEAR << gear << std::endl;

	return true;
}

bool CCarController::SetSpeed(std::istream& args)
{
	if (args.eof())
	{
		m_output << INCORRECT_USING_SET_SPEED << std::endl;
		return true;
	}
	std::string speedStr;
	args >> speedStr;
	int speed;

	try
	{
		speed = std::stoi(speedStr);
	}
	catch (std::invalid_argument e)
	{
		m_output << INVALID_ARGUMENT_SPEED << std::endl;
		return true;
	}

	if (!m_car.SetSpeed(speed)) {
		m_output << ERROR_ARGUMENT_SPEED << std::endl;
		return true;
	}

	m_output << SET_SPEED << speed << std::endl;
	return true;
}

bool CCarController::Quit(std::istream& args)
{
	exit(0);
	return true;
}
