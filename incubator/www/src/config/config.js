import JsonRpcClient from "react-jsonrpc-client";

const config = {
    api: new JsonRpcClient({endpoint: '/rpc'})
};

export default config;