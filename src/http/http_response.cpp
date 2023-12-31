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

static std::string Trim(const std::string &str)
{
	std::string trim(str);
	trim.erase(0, trim.find_first_not_of(" \t\n\r\f\v"));
	trim.erase(trim.find_last_not_of(" \t\n\r\f\v") + 1);
	return trim;
}

const bool &HttpResponse::done(void) const
{
	return done_;
}

void HttpResponse::set_done(const bool &done)
{
	done_ = done;
}

const std::string &HttpResponse::version(void) const
{
	return version_;
}

void HttpResponse::set_version(const std::string &version)
{
	version_ = version;
}

const enum HttpStatus &HttpResponse::status(void) const
{
	return status_;
}

void HttpResponse::set_status(const enum HttpStatus &status)
{
	status_ = status;
}

const std::string HttpResponse::message(const enum HttpStatus &status) const
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

const std::string HttpResponse::header(const std::string& key) const
{
	std::string upper(Trim(key));
	std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
	std::map<std::string, std::string>::const_iterator it = headers_.find(upper);

	return it != headers_.end()? it->second : "";
}

const std::map<std::string, std::string>& HttpResponse::headers(void) const
{
	return headers_;
}

void HttpResponse::add_header(const std::string &key, const std::string &val)
{
	std::string upper(Trim(key));
	std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

	if (upper.compare("SET-COOKIE") == 0)
		cookies_.push_back(Trim(val));
	else
		headers_[upper] = Trim(val);
}

const std::vector<std::string>& HttpResponse::cookies(void) const
{
	return cookies_;
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
	status_ = kOk;
	version_.clear();
	headers_.clear();
	cookies_.clear();
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
	this->add_header("Content-Length", len.str());

	// generate
	res.clear();
	res << version_ << " " << status_ << " " << message(status_) << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
		res << it->first << ": " << it->second << "\r\n";
	for (std::vector<std::string>::const_iterator it = cookies_.begin(); it != cookies_.end(); ++it)
		res << "Set-Cookie: " << *it << "\r\n";
	res << "\r\n";
	res << body_.rdbuf();

#if 1
	// log
std::cerr.clear();
std::cerr << body_.tellp() << "###########################" << std::endl;
	std::cerr << version_ << " " << status_ << " " << message(status_) << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
		std::cerr << it->first << ": " << it->second << "\r\n";
	for (std::vector<std::string>::const_iterator it = cookies_.begin(); it != cookies_.end(); ++it)
		std::cerr << "Set-Cookie: " << *it << "\r\n";
	std::cerr << "\r\n";
	std::cerr << body_.rdbuf()->str().substr(0, 500) << "..." << std::endl;
std::cerr << "###########################" << std::endl;
#endif
	this->Clear();
	return *this;
}
