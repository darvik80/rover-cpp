import React, { useState } from 'react';
import JsonRpcClient from "react-jsonrpc-client";

import {Table} from 'react-bootstrap'

class ShowDateTime extends React.Component {
    constructor() {
        super();
        this.state = {
            curTime : null,
            systemState: null,
        };

        this.api = new JsonRpcClient({endpoint: 'http://localhost:8080/rpc'});

    }
    componentDidMount() {
        setInterval( () => {
            this.setState({
                curTime : new Date().toLocaleString()
            });

            var self = this;
            this.api.request("systemMonitor", "test").then(function (result) {
                self.setState({
                    systemState: result
                })

            });

        },1000)
    }
    render() {
        const { systemState, curTime} = this.state;

        if (systemState == null) {
            return(
                <div>
                    <h2>{this.state.curTime}</h2>
                </div>
            );
        } else {
            return (
                <div>
                    <Table striped bordered hover>
                        <tbody>
                        <tr>
                            <td className="text-right">cpu</td><td>{this.state.systemState.cpuCount}</td>
                        </tr>
                        <tr>
                            <td className="text-right">mem</td><td>{this.state.systemState.physicalMemory/1024/1024/1024} Gb</td>
                        </tr>
                        <tr>
                            <td className="text-right">cpu temp</td><td>{this.state.systemState.cpuTemp}</td>
                        </tr>
                        <tr>
                            <td className="text-right">battery temp</td><td>{this.state.systemState.batteryTemp}</td>
                        </tr>
                        <tr>
                            <td className="text-right">platform</td><td>{this.state.systemState.platform}</td>
                        </tr>
                        </tbody>
                    </Table>
                </div>
            );
        }
    }
}

export default ShowDateTime;