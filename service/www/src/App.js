import React, {Fragment} from 'react';
import {Route, Switch} from 'react-router-dom';
import 'react-dates/initialize';
import './fontAwesome';
import './App.scss';
import {Container, Row} from "reactstrap";
import routes from "./routes/routes";
import {NavMenu} from "./components";
import useNavigation from "./routes/_nav";

const App = () => {
    const navigation = useNavigation();

    return (
        <Fragment>
            <Container fluid>
                <Row>
                    <NavMenu
                        vertical
                        navigation={navigation}
                        className="sidebar d-none d-md-block col-md-2 col-lg-1 bg-light"
                        innerWrapperClassName="sidebar-sticky" />

                    <main role="main" className="col-md-10 col-lg-11 ml-sm-auto bg-light pl-0">
                    </main>
                </Row>
            </Container>
        </Fragment>
);
};

export default App;
