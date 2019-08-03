import {Button, Modal} from 'react-bootstrap'
import React, { useState } from 'react';

import JsonRpcClient from "react-jsonrpc-client/jsonrpcclient"

const ModalDialog = () => {
    const [show, setShow] = useState(false);

    const handleClose = () => setShow(false);
    const handleShow = () => setShow(true);

    var api = new JsonRpcClient({endpoint: 'http://localhost:8080/rpc'});
    api.request("systemMonitor", "test").then(function (result) {
        console.log(result);
    });

    return (
        <div>
            <Button variant="primary" onClick={handleShow}>
                Launch demo modal
            </Button>

            <Modal show={show} onHide={handleClose}>
                <Modal.Header closeButton>
                    <Modal.Title>Modal heading</Modal.Title>
                </Modal.Header>
                <Modal.Body>Woohoo, you're reading this text in a modal!</Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={handleClose}>
                        Close
                    </Button>
                    <Button variant="primary" onClick={handleClose}>
                        Save Changes
                    </Button>
                </Modal.Footer>
            </Modal>
        </div>
    );
};


export default ModalDialog;