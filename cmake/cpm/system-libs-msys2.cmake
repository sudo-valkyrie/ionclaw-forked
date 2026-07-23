# MSYS2/UCRT64: use pre-built Poco (c:/dev/Poco) and system OpenSSL (/ucrt64)
# This avoids building OpenSSL from source (which fails on MSYS2 due to missing netdb.h)
# and avoids recompiling Poco from source every time.

# Set flags first: SSL is available
set(IONCLAW_HAS_SSL TRUE)

# ---- OpenSSL: system-installed in /ucrt64 ----
find_path(OPENSSL_INCLUDE_DIR openssl/ssl.h PATHS /ucrt64/include NO_DEFAULT_PATH)
find_library(OPENSSL_SSL_LIBRARY ssl PATHS /ucrt64/lib NO_DEFAULT_PATH)
find_library(OPENSSL_CRYPTO_LIBRARY crypto PATHS /ucrt64/lib NO_DEFAULT_PATH)

if(NOT OPENSSL_INCLUDE_DIR OR NOT OPENSSL_SSL_LIBRARY)
    message(FATAL_ERROR "System OpenSSL not found in /ucrt64. Install: pacman -S mingw-w64-ucrt-x86_64-openssl")
endif()

message(STATUS "IonClaw: found OpenSSL include=${OPENSSL_INCLUDE_DIR} lib=${OPENSSL_SSL_LIBRARY}")

# Create the imported targets Poco expects
add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
)

add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
)

# ---- Poco: pre-built in c:/dev/Poco ----
set(POCO_DIR "c:/dev/Poco")
set(POCO_INCLUDE_DIR "${POCO_DIR}/include")

if(NOT EXISTS "${POCO_DIR}/lib/libPocoFoundation.a")
    message(FATAL_ERROR "Pre-built Poco not found at ${POCO_DIR}. Build Poco first.")
endif()

# Create a helper macro to define Poco library targets
macro(add_poco_lib name)
    set(_lib_path "${POCO_DIR}/lib/libPoco${name}.a")
    if(EXISTS "${_lib_path}")
        add_library(Poco::${name} STATIC IMPORTED GLOBAL)
        set_target_properties(Poco::${name} PROPERTIES
            IMPORTED_LOCATION "${_lib_path}"
            INTERFACE_INCLUDE_DIRECTORIES "${POCO_INCLUDE_DIR}"
        )
    endif()
endmacro()

# Define the Poco libraries IonClaw needs
add_poco_lib(Foundation)
add_poco_lib(Net)
add_poco_lib(Util)
add_poco_lib(XML)
add_poco_lib(Zip)
add_poco_lib(NetSSL)
add_poco_lib(Crypto)
add_poco_lib(JSON)

# Extra link libraries needed on Windows
set(IONCLAW_SSL_LIBS
    OpenSSL::SSL
    OpenSSL::Crypto
    Poco::NetSSL
    Poco::Crypto
    ws2_32
    iphlpapi
    mswsock
    crypt32
)

# Skip CPM for Poco by marking as added
set(Poco_ADDED TRUE)

# jwt-cpp (header-only): needs SOURCE_DIR for include path
set(jwt-cpp_ADDED TRUE)
# Set it to the pre-built location or skip - IonClaw has its own JwtHelper
add_library(jwt-cpp INTERFACE)
target_include_directories(jwt-cpp INTERFACE ${POCO_DIR}/include)
target_link_libraries(jwt-cpp INTERFACE OpenSSL::SSL OpenSSL::Crypto)
target_compile_definitions(jwt-cpp INTERFACE JWT_DISABLE_PICOJSON)
