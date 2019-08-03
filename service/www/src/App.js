import React, {Fragment} from 'react';
import 'react-dates/initialize';
import './fontAwesome';
import './App.scss';
import {Container, Row} from "reactstrap";

import {Nav, ButtonGroup, Button, Card } from 'react-bootstrap'
import ModalDialog from 'components/ModalDialog'
import ShowDateTime from 'components/ShowDateTime'

const App = () => {
    return (
        <Fragment>
            <Container fluid>
                <Row>
                    <Nav
                        activeKey="/home" onSelect={selectedKey => alert(`selected ${selectedKey}`)}
                        className="flex-column"
                    >
                        <Nav.Item>
                            <Nav.Link href="/home">Active</Nav.Link>
                        </Nav.Item>
                        <Nav.Item>
                            <Nav.Link eventKey="link-1">Link</Nav.Link>
                        </Nav.Item>
                        <Nav.Item>
                            <Nav.Link eventKey="disabled" disabled>
                                Disabled
                            </Nav.Link>
                        </Nav.Item>
                    </Nav>
                    <main role="main">
                        <Row>
                            <ButtonGroup aria-label="Basic example">
                                <Button variant="secondary">Left</Button>
                                <Button variant="secondary">Middle</Button>
                                <Button variant="secondary">Right</Button>
                            </ButtonGroup>
                        </Row>
                        <Row>
                            <Card style={{ width: '18rem' }}>
                                <Card.Img variant="top" src="holder.js/100px180" />
                                <Card.Body>
                                    <Card.Title>Card Title</Card.Title>
                                    <Card.Text>
                                        Some quick example text to build on the card title and make up the bulk of
                                        the card's content.
                                    </Card.Text>
                                    <Button variant="primary">Go somewhere</Button>
                                </Card.Body>
                            </Card>
                        </Row>
                        <Row><ModalDialog/></Row>
                        <Row><ShowDateTime/></Row>

                    </main>
                </Row>
            </Container>
        </Fragment>
    );
};

export default App;
