import JsonRpcClient from "react-jsonrpc-client";

const config = {
    api: new JsonRpcClient({endpoint: process.env.NODE_ENV.trim() === 'production' ? '/rpc' : 'http://localhost:3333'})
};

export default config;