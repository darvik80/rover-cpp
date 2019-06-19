import React, {Fragment} from 'react';
import {Route, Switch} from 'react-router-dom';
import {Container, Row} from 'reactstrap';

import useNavigation from 'routes/_nav';
import routes from 'routes/routes';
import {NavMenu} from 'components';
import DefaultHeader from './DefaultHeader';


const DefaultLayout = (props) => {
  const navigation = useNavigation();

  return (
    <Fragment>
      <DefaultHeader/>
      <Container fluid>
        <Row>
          <NavMenu
            vertical
            navigation={navigation}
            className="sidebar d-none d-md-block col-md-2 col-lg-1 bg-light"
            innerWrapperClassName="sidebar-sticky" />
          <main role="main" className="col-md-10 col-lg-11 ml-sm-auto bg-light pl-0">
            <Switch>
              {routes.filter(route => route.page).map((route, idx) => (
                <Route key={idx} path={route.page.URL} exact={route.exact} name={route.page.Label}/>
              ))}
            </Switch>
            <br />
            <br />
            <br />
            <br />
            <br />
          </main>
        </Row>
      </Container>
    </Fragment>
  );
};

export default DefaultLayout;
