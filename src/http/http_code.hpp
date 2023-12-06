#ifndef SRC_HTTP_HTTP_CODE_HPP_
# define SRC_HTTP_HTTP_CODE_HPP_

enum HttpStep {
	kParseStart,
	kParseHeader,
	kParseBody,
	kParseChunkLen,
	kParseChunkBody,
	kParseDone,
	kParseFail
};

enum HttpStatus {
	// 1xx Informational
	kContinue = 100,
	kSwitchingProtocols = 101,
	kProcessing = 102,
	kEarlyHints = 103,

	// 2xx Success
	kOk = 200,
	kCreated = 201,
	kAccepted = 202,
	kNonAuthoritativeInformation = 203,
	kNoContent = 204,
	kResetContent = 205,
	kPartialContent = 206,
	kMultiStatus = 207,
	kAlreadyReported = 208,
	kIMUsed = 226,

	// 3xx Redirection
	kMultipleChoices = 300,
	kMovedPermanently = 301,
	kFound = 302,
	kSeeOther = 303,
	kNotModified = 304,
	kUseProxy = 305,
	kTemporaryRedirect = 307,
	kPermanentRedirect = 308,

	// 4xx Client Error
	kBadRequest = 400,
	kUnauthorized = 401,
	kPaymentRequired = 402,
	kForbidden = 403,
	kNotFound = 404,
	kMethodNotAllowed = 405,
	kNotAcceptable = 406,
	kProxyAuthenticationRequired = 407,
	kRequestTimeout = 408,
	kConflict = 409,
	kGone = 410,
	kLengthRequired = 411,
	kPreconditionFailed = 412,
	kPayloadTooLarge = 413,
	kRequestURITooLong = 414,
	kUnsupportedMediaType = 415,
	kRangeNotSatisfiable = 416,
	kExpectationFailed = 417,
	kTeapot = 418,
	kMisdirectedRequest = 421,
	kUnprocessableContent = 422,
	kLocked = 423,
	kFailedDependency = 424,
	kTooEarly = 425,
	kUpgradeRequired = 426,
	kPreconditionRequired = 428,
	kTooManyRequests = 429,
	kRequestHeaderFieldsTooLarge = 431,
	kConnectionClosedWithoutResponse = 444,
	kUnavailableForLegalReasons = 451,
	kClientClosedRequest = 499,

	// 5xx Server Error
	kInternalServerError = 500,
	kNotImplemented = 501,
	kBadGateway = 502,
	kServiceUnavailable = 503,
	kGatewayTimeout = 504,
	kHttpVersionNotSupported = 505,
	kVariantAlsoNegotiates = 506,
	kInsufficientStorage = 507,
	kLoopDetected = 508,
	kNotExtended = 510,
	kNetworkAuthenticationRequired = 511,
	kNetworkConnectTimeoutError = 599
};

#endif /* SRC_HTTP_HTTP_CODE_HPP_ */

