import React, {useEffect, useState} from 'react';
import {Table} from "react-bootstrap";

function Monitoring() {
    const [curTime, setCurTime] = useState(null);

    useEffect(() => {
        const id = setInterval( () => {
            setCurTime(new Date().toLocaleString());
        },1000);

        return () => {
            clearInterval(id);
        };
    });


    return (
        <div>
            <Table striped bordered hover>
                <tbody>
                <tr>
                    <td className="text-right">time</td><td>{curTime}</td>
                </tr>
                </tbody>
            </Table>
        </div>
    );
}