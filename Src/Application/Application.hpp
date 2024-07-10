#pragma once

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

/// <summary>
/// An abstract base class representing an application.
/// Defines methods for the main functionality of the application.
/// </summary>
class Application
{
	public:
		/// <summary>
		/// main page of the application
		/// </summary>
		virtual void Main() = 0;

		/// <summary>
		/// Gets the status indicating whether the main page need to be closed.
		/// </summary>
		virtual bool IsOpen() const = 0;
};

#endif 
