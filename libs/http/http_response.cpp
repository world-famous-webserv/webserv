#include "http_response.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

HttpResponse::~HttpResponse(void)
{
}

HttpResponse::HttpResponse(void)
{
	this->Clear();
}

/* ************************************************************************** */
// get / set
/* ************************************************************************** */

bool HttpResponse::done(void) const
{
	return done_;
}

void HttpResponse::set_done(bool done)
{
	done_ = done;
}

std::string HttpResponse::version(void) const
{
	return version_;
}

void HttpResponse::set_version(const std::string version)
{
	version_ = version;
}

enum HttpStatus HttpResponse::status(void) const
{
	return status_;
}

void HttpResponse::set_status(enum HttpStatus status)
{
	status_ = status;
}

std::string HttpResponse::message(enum HttpStatus status) const
{
	switch (status)
	{
		// 1xx Informational
		case kContinue: return "Continue";
		case kSwitchingProtocols: return "Switching Protocols";
		case kProcessing: return "Processing";
		case kEarlyHints: return "Early Hints";

		// 2xx Success
		case kOk: return "Ok";
		case kCreated: return "Created";
		case kAccepted: return "Accepted";
		case kNonAuthoritativeInformation: return "Non-Authoritative Information";
		case kNoContent: return "No Content";
		case kResetContent: return "Reset Content";
		case kPartialContent: return "Partial Content";
		case kMultiStatus: return "Multi-Status";
		case kAlreadyReported: return "Already Reported";
		case kIMUsed: return "IM Used";

		// 3xx Redirection
		case kMultipleChoices: return "Multiple Choices";
		case kMovedPermanently: return "Moved Permanently";
		case kFound: return "Found";
		case kSeeOther: return "See Other";
		case kNotModified: return "Not Modified";
		case kUseProxy: return "Use Proxy";
		case kTemporaryRedirect: return "Temporary Redirect";
		case kPermanentRedirect: return "Permanent Redirect";

		// 4xx Client Error
		case kBadRequest: return "Bad Request";
		case kUnauthorized: return "Unauthorized";
		case kPaymentRequired: return "Payment Required";
		case kForbidden: return "Forbidden";
		case kNotFound: return "Not Found";
		case kMethodNotAllowed: return "Method Not Allowed";
		case kNotAcceptable: return "Not Acceptable";
		case kProxyAuthenticationRequired: return "Proxy Authentication Required";
		case kRequestTimeout: return "Request Timeout";
		case kConflict: return "Conflict";
		case kGone: return "Gone";
		case kLengthRequired: return "Length Required";
		case kPreconditionFailed: return "Precondition Failed";
		case kPayloadTooLarge: return "Payload Too Large";
		case kRequestURITooLong: return "Request URI Too Long";
		case kUnsupportedMediaType: return "Unsupported Media Type";
		case kRangeNotSatisfiable: return "Range Not Satisfiable";
		case kExpectationFailed: return "Expectation Failed";
		case kTeapot: return "I'm a teapot";
		case kMisdirectedRequest: return "Misdirected Request";
		case kUnprocessableContent: return "Unprocessable Content";
		case kLocked: return "Locked";
		case kFailedDependency: return "Failed Dependency";
		case kTooEarly: return "Too Early";
		case kUpgradeRequired: return "Upgrade Required";
		case kPreconditionRequired: return "Precondition Required";
		case kTooManyRequests: return "Too Many Requests";
		case kRequestHeaderFieldsTooLarge: return "Request Header Fields Too Large";
		case kConnectionClosedWithoutResponse: return "Connection Closed Without Response";
		case kUnavailableForLegalReasons: return "Unavailable For Legal Reasons";
		case kClientClosedRequest: return "Client Closed Request";

		// 5xx Server Error
		case kInternalServerError: return "Internal Server Error";
		case kNotImplemented: return "Not Implemented";
		case kBadGateway: return "Bad Gateway";
		case kServiceUnavailable: return "Service Unavailable";
		case kGatewayTimeout: return "Gateway Timeout";
		case kHttpVersionNotSupported: return "Http Version Not Supported";
		case kVariantAlsoNegotiates: return "Variant Also Negotiates";
		case kInsufficientStorage: return "Insufficient Storage";
		case kLoopDetected: return "Loop Detected";
		case kNotExtended: return "Not Extended";
		case kNetworkAuthenticationRequired: return "Network Authentication Required";
		case kNetworkConnectTimeoutError: return "Network Connect Timeout Error";
		default:
			break;
	}
	return "Unkown";
}

std::string HttpResponse::header(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator it = headers_.find(key);
	return it != headers_.end()? it->second : "";
}

void HttpResponse::add_header(const std::string key, const std::string val)
{
	headers_[key] = val;
}

std::stringstream& HttpResponse::body(void)
{
	return body_;
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void HttpResponse::Clear(void)
{
	done_ = false;
	status_ = kNotFound;
	version_.clear();
	headers_.clear();
	body_.clear();
	body_.str("");
}

#include <iostream>
HttpResponse& HttpResponse::operator>>(std::stringstream& res)
{
	// calc body len
	body_.clear();
	std::stringstream len;
	len << body_.tellp();
	headers_["Content-Length"] = len.str();

	// generate
	res.clear();
	res << version_ << " " << status_ << " " << message(status_) << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
		res << it->first << ": " << it->second << "\r\n";
	res << "\r\n";
	res << body_.str();
	this->Clear();
	return *this;
}
